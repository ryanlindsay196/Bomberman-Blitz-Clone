#pragma once

#include <assert.h>

struct MetaType
{
	virtual double ToNumber(void* v) const = 0;
	virtual const char* Name() const = 0;
	virtual size_t SizeOf() const = 0;
	virtual void Cast(void* to, void* from, const MetaType& fromType) const = 0;
};

struct VoidMetaType : public MetaType
{
	virtual double ToNumber(void* v) const override { return 0; }
	virtual const char* Name() const override { return (char*)"void"; }
	virtual size_t SizeOf() const override { return 0; }
	virtual void Cast(void* to, void* from, const MetaType& fromType) const;
};

struct IntMetaType : public MetaType
{
	virtual double ToNumber(void* v) const override { return *(int*)v; }
	virtual const char* Name() const override { return "int"; }
	virtual size_t SizeOf() const override { return sizeof(int); }
	virtual void Cast(void* to, void* from, const MetaType& fromType) const;
};

struct FloatMetaType : public MetaType
{
	virtual double ToNumber(void* v) const  override { return *(float*)v; }
	virtual const char* Name() const  override { return "float"; }
	virtual size_t SizeOf() const  override { return sizeof(float); }
	virtual void Cast(void* to, void* from, const MetaType& fromType) const;
};

struct DoubleMetaType : public MetaType
{
	virtual double ToNumber(void* v) const  override { return *(double*)v; }
	virtual const char* Name() const override { return "double"; }
	virtual size_t SizeOf() const override { return sizeof(double); }
	virtual void Cast(void* to, void* from, const MetaType& fromType) const;
};

template <typename T>
class MetaTypeTemplate : public MetaType
{
	virtual double ToNumber(void* v) const { return 0; };
	virtual const char* Name() const { return (char*)"MetaTypeTemplate"; };
	virtual size_t SizeOf() const { return sizeof(T); };
	virtual void Cast(void* to, void* from, const MetaType& fromType) const {};
};

const MetaType& GetGlobalMetaType(const int&);
const MetaType& GetGlobalMetaType(const float&);
const MetaType& GetGlobalMetaType(const double&);
const MetaType& GetGlobalMetaType(void*);
template<typename T>
const MetaType & GetGlobalMetaType(const T &)
{
	static MetaTypeTemplate<T> metaType = MetaTypeTemplate<T>();
	return metaType;
}

template <typename T>
class AutoLister
{
public:
	AutoLister()
	{
		m_next = Head();
		Head() = static_cast<T*>(this);
	}

	T* Next() { return m_next; }
	const T* Next() const { return m_next; }

	static T*& Head() { static T* p = 0; return p; }

private:
	T* m_next;
};

#define CreateClassMetadata(Class)\
static int GetClassID() { return __COUNTER__; }\
static char* GetClassName() { return (char*)#Class; }\
struct MetaVariable : public AutoLister<MetaVariable>\
{\
public:\
	template <typename Var>\
	MetaVariable(Var* var, const char* name_, unsigned int offset_, unsigned int size_) :\
		name(name_),\
		offset(offset_),\
		size(size_),\
		metaType(GetGlobalMetaType(*var))\
		{}\
	const char* GetName() { return name; }\
	const unsigned int GetOffset() { return offset; }\
	const unsigned int GetSize() { return size; }\
	const MetaType& GetMetaType() { return metaType; }\
private:\
	const char* const name;\
	const unsigned int offset;\
	const unsigned int size;\
	const MetaType& metaType;\
};\
\
class FunctionSignature\
{\
public:\
	template <typename Ret>\
	FunctionSignature(Ret(Class::*)())\
	{\
		ret = &GetMetaTypeByType<Ret>();\
		args = 0;\
		argCount = 0;\
	}\
\
	template <typename Ret, typename Arg0>\
	FunctionSignature(Ret(Class::*)(Arg0))\
	{\
		ret = &GetMetaTypeByType<Ret>();\
		static const MetaType* arguments[1] = { &GetMetaTypeByType<Arg0>() };\
		args = arguments;\
		argCount = 1;\
	}\
\
	template <typename Ret, typename Arg0, typename Arg1>\
	FunctionSignature(Ret(Class::*)(Arg0, Arg1))\
	{\
		ret = &GetMetaTypeByType<Ret>();\
		static const MetaType* arguments[2] = { &GetMetaTypeByType<Arg0>(), &GetMetaTypeByType<Arg1>() };\
		args = arguments;\
		argCount = 2;\
	}\
\
	template <typename Ret, typename Arg0, typename Arg1, typename Arg2>\
	FunctionSignature(Ret(Class::*)(Arg0, Arg1, Arg2))\
	{\
		ret = &GetMetaTypeByType<Ret>();\
		static const MetaType* arguments[3] = { &GetMetaTypeByType<Arg0>(), &GetMetaTypeByType<Arg1>(), &GetMetaTypeByType<Arg2>() };\
		args = arguments;\
		argCount = 3;\
	}\
\
	template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3>\
	FunctionSignature(Ret(Class::*)(Arg0, Arg1, Arg2, Arg3))\
	{\
		ret = &GetMetaTypeByType<Ret>();\
		static const MetaType* arguments[4] = { &GetMetaTypeByType<Arg0>(), &GetMetaTypeByType<Arg1>(), &GetMetaTypeByType<Arg2>() , &GetMetaTypeByType<Arg3>() };\
		args = arguments;\
		argCount = 4;\
	}\
\
	const MetaType* RetType() const { return ret; }\
	const MetaType* ArgType(int idx) const { return args[idx]; }\
	int ArgCount() const { return argCount; }\
\
private:\
	const MetaType* ret;\
	const MetaType** args;\
	int argCount;\
};\
\
struct MetaFunction : public AutoLister<MetaFunction>\
{\
public:\
	template <typename Fun>\
	MetaFunction(const char* name, Fun fun) :\
		m_name(name),\
		m_sig(fun)\
	{\
		m_fun = (void(Class::*)())fun;\
		m_applyWrapper = &ApplyWrapper<Fun, Class>;\
		m_metaFunctionID = 0;\
		InitializeID();\
	}\
\
	const char* Name() const { return m_name; }\
	const MetaType* RetType() const { return m_sig.RetType(); }\
	const MetaType* ArgType(int idx) const { return m_sig.ArgType(idx); }\
	int ArgCount() const { return m_sig.ArgCount(); };\
\
	void Apply(Class* entity, Variable ret, Variable* args, int argCount) const\
	{\
		m_applyWrapper(entity, m_fun, ret, args, argCount);\
	}\
\
	void InitializeID()\
	{\
		static unsigned int maximumID = 0;\
		if (m_metaFunctionID == 0)\
		{\
			maximumID++;\
			m_metaFunctionID = maximumID;\
		}\
	}\
\
	inline unsigned int GetMetaFunctionID() { return m_metaFunctionID; }\
\
private:\
	unsigned int m_metaFunctionID;\
	const char* m_name;\
	FunctionSignature m_sig;\
	void(Class::*m_fun)();\
	void(*m_applyWrapper)(Class* entity, void(Class::*fun)(), Variable, Variable*, int);\
};\
\
virtual BaseObject::MetaFunction* GetMetaFunctionByID(unsigned int id)\
{\
	for(Class::MetaFunction* metaFunction = Class::MetaFunction::Head(); metaFunction; metaFunction = metaFunction->Next())\
	{\
		if(id == metaFunction->GetMetaFunctionID())\
		{\
			return (BaseObject::MetaFunction*)metaFunction;\
		}\
	}\
	return nullptr;\
}\
static Class::MetaFunction* GetMetaFunctionByName(const char* Name)\
{\
	for (Class::MetaFunction* mf = Class::MetaFunction::Head(); mf; mf = mf->Next())\
	{\
		if (mf->Name() == Name)\
		{\
			return (Class::MetaFunction*)mf;\
		}\
	}\
\
	assert(false && "No meta function has been found. Check that the Name parameter matches a meta function instance.");\
	return nullptr;\
}

template <typename Type>
const MetaType& GetMetaTypeByType()
{
	Type* t = nullptr;
	return GetGlobalMetaType(*t);
};

template <>
const MetaType& GetMetaTypeByType<void>();

struct Variable
{
	Variable() : v(0), m(&GetMetaTypeByType<void>()) {}

	template<typename T>
	Variable(const T& t) : v((void*)&t), m(&GetGlobalMetaType(t)) {}

	void* v;
	const MetaType* m;
};

template <typename Fun, class Class>
inline void ApplyWrapper(Class* classInstance, void(Class::*fun)(), Variable ret, Variable* args, int argCount)
{
	Apply(classInstance, (Fun)fun, ret, args, argCount);
}

template <class Class, typename Ret>
inline void Apply(Class* classInstance, Ret(Class::*fun) (), Variable ret, Variable* args, int argCount)
{
	assert(argCount == 0);
	Ret r = (classInstance->*fun)();
	ret.m->Cast(ret.v, &r, GetMetaTypeByType<void>());
}
template <class Class>
inline void Apply(Class* classInstance, void(Class::*fun) (), Variable ret, Variable* args, int argCount)
{
	assert(argCount == 0);
	(classInstance->*fun)();
}

template <class Class, typename Ret, typename Arg0>
inline void Apply(Class* classInstance, Ret(Class::*fun) (Arg0), Variable ret, Variable* args, int argCount)
{
	assert(argCount == 1);
	Ret r = (classInstance->*fun)(*(Arg0*)args[0].v);
	ret.m->Cast(ret.v, &r, GetGlobalMetaType(r));
}
template <class Class, typename Arg0>
inline void Apply(Class* classInstance, void(Class::*fun) (Arg0), Variable ret, Variable* args, int argCount)
{
	assert(argCount == 1);
	(classInstance->*fun)(*(Arg0*)args[0].v);
}

template <class Class, typename Ret, typename Arg0, typename Arg1>
inline void Apply(Class* classInstance, Ret(Class::*fun) (Arg0, Arg1), Variable ret, Variable* args, int argCount)
{
	assert(argCount == 2);
	Ret r = (classInstance->*fun)(*(Arg0*)args[0].v, *(Arg1*)args[1].v);
	ret.m->Cast(ret.v, &r, GetGlobalMetaType(r));
}
template <class Class, typename Arg0, typename Arg1>
inline void Apply(Class* classInstance, void(Class::*fun) (Arg0, Arg1), Variable ret, Variable* args, int argCount)
{
	assert(argCount == 2);
	(classInstance->*fun)(*(Arg0*)args[0].v, *(Arg1*)args[1].v);
}

template <class Class, typename Ret, typename Arg0, typename Arg1, typename Arg2>
inline void Apply(Class* classInstance, Ret(Class::*fun)(Arg0, Arg1, Arg2), Variable ret, Variable * args, int argCount)
{
	assert(argCount == 3);
	Ret r = (classInstance->*fun)(*(Arg0*)args[0].v, *(Arg1*)args[1].v, *(Arg2*)args[2].v);
	ret.m->Cast(ret.v, &r, GetGlobalMetaType(r));
}
template <class Class, typename Arg0, typename Arg1, typename Arg2>
inline void Apply(Class* classInstance, void(Class::*fun)(Arg0, Arg1, Arg2), Variable ret, Variable * args, int argCount)
{
	assert(argCount == 3);
	(classInstance->*fun)(*(Arg0*)args[0].v, *(Arg1*)args[1].v, *(Arg2*)args[2].v);
}

template <class Class, typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
inline void Apply(Class* classInstance, Ret(Class::*fun) (Arg0, Arg1, Arg2, Arg3), Variable ret, Variable* args, int argCount)
{
	assert(argCount == 4);
	Ret r = (classInstance->*fun)(*(Arg0*)args[0].v, *(Arg1*)args[1].v, *(Arg2*)args[2].v, *(Arg3*)args[3].v);
	ret.m->Cast(ret.v, &r, GetGlobalMetaType(r));
}
template <class Class, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
inline void Apply(Class* classInstance, void(Class::*fun) (Arg0, Arg1, Arg2, Arg3), Variable ret, Variable* args, int argCount)
{
	assert(argCount == 4);
	(classInstance->*fun)(*(Arg0*)args[0].v, *(Arg1*)args[1].v, *(Arg2*)args[2].v, *(Arg3*)args[3].v);
}


#define Networked(AuthorityType, var)\
RegisterNetworkedVariable(AuthorityType, (BaseObject::MetaVariable*)&mv_##var);

#define CreateVariableMetadata(class, var, ...)\
static MetaVariable mv_##var = MetaVariable(&var, #var, offsetof(class, var), sizeof(var));\
__VA_ARGS__

#define CreateClassFunctionMetadata(Class, func)\
static MetaFunction mf_##func = MetaFunction(#func, &Class##::func)

#define CreateTemplatedClassFunctionMetadata(Class, func, myTemplates)\
static MetaFunction mf_##func = MetaFunction(#func #myTemplates, &Class##::func##myTemplates)