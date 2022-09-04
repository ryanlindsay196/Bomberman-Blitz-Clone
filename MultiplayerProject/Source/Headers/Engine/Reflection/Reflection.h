#pragma once

#define CreateClassReflectionMetadata(Class)\
static int GetClassID() { return __COUNTER__; }\
static char* GetClassName() { return (char*)#Class; }

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

struct ReflectionVariable : public AutoLister<ReflectionVariable>
{
	ReflectionVariable(const char* name_, unsigned int offset_, unsigned int size_, AuthorityType authorityType_) :
		name(name_),
		offset(offset_),
		size(size_),
		authorityType(authorityType){}
	const char* const name;
	unsigned int offset;
	unsigned int size;
	AuthorityType authorityType;
};

#define CreateVariableMetadata(class, var, authType)\
ReflectionVariable rv_##var = ReflectionVariable(#var, offsetof(class, var), sizeof(var), authType);\

//#define CreateVariableReflectionMetadata(class, var, authType)\
//ReflectionVariable& GetReflectionVariable_##var()\
//{\
//	static ReflectionVariable rv = ReflectionVariable(#var, offsetof(class, var), sizeof(var), authType);\
//	return rv;\
//}

class TempClass
{
	CreateVariableMetadata(TempClass, tempInt, AuthorityType::OwningClient);
	int tempInt;
	CreateVariableMetadata(TempClass, tempUInt, AuthorityType::OwningClient);
	unsigned int tempUInt;
	CreateVariableMetadata(TempClass, s, AuthorityType::OwningClient);
	int s;
public:
};

class TempClass2
{
	CreateVariableMetadata(TempClass2, tempInt2, AuthorityType::OwningClient);
	int tempInt2;
	CreateVariableMetadata(TempClass2, tempUInt2, AuthorityType::OwningClient);
	unsigned int tempUInt2;
	CreateVariableMetadata(TempClass2, s2, AuthorityType::OwningClient);
	int s2;
public:
	//TODO: delete this
	void PrintReflectedVars()
	{
		printf("Printing reflected vars...\n");
		for (ReflectionVariable* rv = rv_s2.Head(); rv; rv = rv->Next())
		{
			printf("var name %s, offset = %i\n", rv->name, rv->offset);
		}
		printf("\n");
	}
};