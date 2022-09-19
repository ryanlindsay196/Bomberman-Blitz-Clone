#include "Engine/Reflection/Reflection.h"

const MetaType& GetGlobalMetaType(int)
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
