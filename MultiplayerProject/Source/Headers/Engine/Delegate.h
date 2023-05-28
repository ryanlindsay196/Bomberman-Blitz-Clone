#pragma once
#include "RawPointerTracker.h"

template<typename ReturnType, typename... Args>
class NullFunctionPtr
{
public:
	virtual ReturnType Execute(Args&&... args) { return ReturnType(); }
	virtual bool IsBound() { return false; }
};

template<typename ...Args>
class NullFunctionPtr<void, Args...>
{
public:
	virtual void Execute(Args&&... args) {}
	virtual bool IsBound() { return false; }
};

template<class MyClass, typename ReturnType, typename... Args>
class MemberFunctionPtr : public NullFunctionPtr<ReturnType, Args...>
{
public:
	MemberFunctionPtr(MyClass* inObject, ReturnType(MyClass::*delegateToBind)(Args...)) :
		boundObject(inObject),
		myDelegate(delegateToBind),
		boundObjectHandleLocation(inObject->GetRawPointerHandle()),
		cachedPointerHandle(*(inObject->GetRawPointerHandle()))
	{}

	virtual bool IsBound() override { 		
		bool isHandleValid = boundObjectHandleLocation && boundObjectHandleLocation->index == cachedPointerHandle.index;
		return isHandleValid && myDelegate != nullptr; 
	}

	virtual ReturnType Execute(Args&&...args) override
	{
		if (IsBound())
		{
			return (boundObject->*myDelegate)(std::forward<Args>(args)...);
		}
		return ReturnType();
	}

private:
	MyClass* boundObject;
	RawPointerHandle* boundObjectHandleLocation;
	RawPointerHandle cachedPointerHandle;

	ReturnType(MyClass::*myDelegate)(Args...);
};

template<class MyClass, typename... Args>
class MemberFunctionPtr<MyClass, void, Args...> : public NullFunctionPtr<void, Args...>
{
public:
	MemberFunctionPtr(MyClass* inObject, void(MyClass::*delegateToBind)(Args...)) :
		boundObject(inObject),
		myDelegate(delegateToBind),
		boundObjectHandleLocation(inObject->GetRawPointerHandle()),
		cachedPointerHandle(*(inObject->GetRawPointerHandle()))
	{}

	virtual bool IsBound() override 
	{ 
		bool isHandleValid = boundObjectHandleLocation && boundObjectHandleLocation->index == cachedPointerHandle.index;
		return isHandleValid && myDelegate != nullptr; 
	}

	virtual void Execute(Args&&...args) override
	{
		if (IsBound())
		{
			(boundObject->*myDelegate)(std::forward<Args>(args)...);
		}
	}

private:
	MyClass* boundObject;
	RawPointerHandle* boundObjectHandleLocation;
	RawPointerHandle cachedPointerHandle;
	void(MyClass::*myDelegate)(Args...);
};

template<typename ReturnType, typename... Args>
class StaticFunctionPtr : public NullFunctionPtr<ReturnType, Args...>
{
public:
	StaticFunctionPtr(ReturnType(*delegateToBind)(Args...)) :
		myDelegate(delegateToBind)
	{}


	virtual ReturnType Execute(Args&&... args) override
	{
		if (IsBound())
		{
			return (*myDelegate)(std::forward<Args>(args)...);
		}
		return ReturnType();
	}
	virtual bool IsBound() override { return myDelegate != nullptr; }

private:
	ReturnType(*myDelegate)(Args...);
};

template<typename... Args>
class StaticFunctionPtr<void, Args...> : public NullFunctionPtr<void, Args...>
{
public:
	StaticFunctionPtr(void(*delegateToBind)(Args...)) :
		myDelegate(delegateToBind)
	{}


	virtual void Execute(Args&&... args) override
	{
		if (IsBound())
		{
			(*myDelegate)(std::forward<Args>(args)...);
		}
	}
	virtual bool IsBound() override { return myDelegate != nullptr; }

private:
	void(*myDelegate)(Args...);
};

template<typename ReturnType, typename... Args>
class SingleCastDelegate
{
public:
	~SingleCastDelegate()
	{
		CleanupFunctionPtr();
	}

	template <typename MyClass>
	void BindMemberFunction(MyClass* inObject, ReturnType(MyClass::*delegateToBind)(Args...))
	{
		CleanupFunctionPtr();
		functionType = new MemberFunctionPtr<MyClass, ReturnType, Args...>(inObject, delegateToBind);
	}

	void BindStaticFunction(ReturnType(*delegateToBind)(Args...))
	{
		CleanupFunctionPtr();
		functionType = new StaticFunctionPtr<ReturnType, Args...>(delegateToBind);
	}

	void Unbind()
	{
		CleanupFunctionPtr();
		functionType = NullFunctionPtr<Args...>();
	}

	bool IsBound() { return functionType && functionType->IsBound(); }

	ReturnType Execute(Args... args)
	{
		if (IsBound())
		{
			return functionType->Execute(std::forward<Args>(args)...);
		}
		else if (functionType)
		{
			CleanupFunctionPtr();
		}

		return ReturnType();
	}
private:
	void CleanupFunctionPtr()
	{
		if (functionType)
		{
			delete(functionType);
		}
		functionType = nullptr;
	}

	NullFunctionPtr<ReturnType, Args...>* functionType;
};

template<typename... Args>
class SingleCastDelegate<void, Args...>
{
public:
	~SingleCastDelegate()
	{
		CleanupFunctionPtr();
	}

	template <typename MyClass>
	void BindMemberFunction(MyClass* inObject, void(MyClass::*delegateToBind)(Args...))
	{
		CleanupFunctionPtr();
		functionType = new MemberFunctionPtr<MyClass, void, Args...>(inObject, delegateToBind);
	}

	void BindStaticFunction(void(*delegateToBind)(Args...))
	{
		CleanupFunctionPtr();
		functionType = new StaticFunctionPtr<void, Args...>(delegateToBind);
	}

	void Unbind()
	{
		CleanupFunctionPtr();
		functionType = NullFunctionPtr<Args...>();
	}

	bool IsBound() { return functionType && functionType->IsBound(); }

	void Execute(Args... args)
	{
		if (IsBound())
		{
			functionType->Execute(std::forward<Args>(args)...);
		}
		else if(functionType)
		{
			CleanupFunctionPtr();
		}
	}

private:
	void CleanupFunctionPtr()
	{
		if (functionType)
		{
			delete(functionType);
		}
		functionType = nullptr;
	}

	NullFunctionPtr<void, Args...>* functionType;
};
