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
	ReflectionVariable(const char* name_, unsigned int offset_, unsigned int size_, AuthorityType authorityType_) :\
		name(name_),\
		offset(offset_),\
		size(size_),\
		authorityType(authorityType){}\
	const char* const name;\
	unsigned int offset;\
	unsigned int size;\
	AuthorityType authorityType;\
};

#define CreateVariableMetadata(class, var, authType)\
static ReflectionVariable rv_##var = ReflectionVariable(#var, offsetof(class, var), sizeof(var), authType);