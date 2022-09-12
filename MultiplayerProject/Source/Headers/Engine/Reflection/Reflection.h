#pragma once

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

#define CreateClassReflectionMetadata(Class)\
static int GetClassID() { return __COUNTER__; }\
static char* GetClassName() { return (char*)#Class; }\
struct ReflectionVariable : public AutoLister<ReflectionVariable>\
{\
public:\
	ReflectionVariable(const char* name_, unsigned int offset_, unsigned int size_) :\
		name(name_),\
		offset(offset_),\
		size(size_) {}\
	const char* const name;\
	const unsigned int offset;\
	const unsigned int size;\
};

#define Networked(AuthorityType, var)\
RegisterNetworkedVariable(AuthorityType, (void*)&rv_##var);

#define CreateVariableMetadata(class, var, ...)\
static ReflectionVariable rv_##var = ReflectionVariable(#var, offsetof(class, var), sizeof(var));\
__VA_ARGS__