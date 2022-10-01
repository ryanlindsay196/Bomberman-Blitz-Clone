#include "Engine/Reflection/Reflection.h"

const MetaType& GetGlobalMetaType(const int&)
{
	static IntMetaType intMetaType;  
	return intMetaType;
}

const MetaType & GetGlobalMetaType(const float &)
{
	static FloatMetaType floatMetaType;  
	return floatMetaType;
}

const MetaType & GetGlobalMetaType(const double &)
{
	static DoubleMetaType doubleMetaType;
	return doubleMetaType;
}

const MetaType& GetGlobalMetaType(void*)
{
	static VoidMetaType metaType;
	return metaType;
}

template<>
const MetaType& GetMetaTypeByType<void>()
{
	void* v = nullptr;
	return GetGlobalMetaType(v);
}

void IntMetaType::Cast(void * to, void * from, const MetaType & fromType) const
{
	*(int*)to = (int)fromType.ToNumber(from);
}

void FloatMetaType::Cast(void * to, void * from, const MetaType & fromType) const
{
	*(float*)to = (float)fromType.ToNumber(from);
}

void DoubleMetaType::Cast(void * to, void * from, const MetaType & fromType) const
{
	*(double*)to = (double)fromType.ToNumber(from);
}

void VoidMetaType::Cast(void * to, void * from, const MetaType & fromType) const
{
}