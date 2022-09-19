#pragma once

struct MetaType
{
	virtual double ToNumber(void* v) const = 0;
	virtual const char* Name() const = 0;
	virtual size_t SizeOf() const = 0;
};

struct IntMetaType : public MetaType
{
	virtual double ToNumber(void* v) const override { return *(int*)v; }
	virtual const char* Name() const override { return "int"; }
	virtual size_t SizeOf() const override { return sizeof(int); }
};

struct FloatMetaType : public MetaType
{
	virtual double ToNumber(void* v) const  override { return *(float*)v; }
	virtual const char* Name() const  override { return "float"; }
	virtual size_t SizeOf() const  override { return sizeof(float); }
};

struct DoubleMetaType : public MetaType
{
	virtual double ToNumber(void* v) const  override { return *(double*)v; }
	virtual const char* Name() const override { return "double"; }
	virtual size_t SizeOf() const override { return sizeof(double); }
};

const MetaType& GetGlobalMetaType(int);
const MetaType& GetGlobalMetaType(const float&);
const MetaType& GetGlobalMetaType(const double&);

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
};

#define Networked(AuthorityType, var)\
RegisterNetworkedVariable(AuthorityType, (BaseObject::MetaVariable*)&mv_##var);

#define CreateVariableMetadata(class, var, ...)\
static MetaVariable mv_##var = MetaVariable(&var, #var, offsetof(class, var), sizeof(var));\
__VA_ARGS__