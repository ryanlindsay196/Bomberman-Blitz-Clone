#pragma once

#include "rapidjson/document.h"
#include "Reflection/Reflection.h"
#include "Engine/Delegate.h"
#if RunInEngine
class Engine;
#endif

class JsonDataPopulator
{
private:
	static void CopyVariableFromJSON(Variable & destinationVar, const rapidjson::GenericArray<false, rapidjson::Value::ValueType>::PlainType & sourceJSON)
	{
		if (sourceJSON.IsInt())
		{
			int intToCopy = sourceJSON.GetInt();
			memcpy(destinationVar.v, &intToCopy, destinationVar.m->SizeOf());
		}
		else if (sourceJSON.IsFloat())
		{
			float floatToCopy = sourceJSON.GetFloat();
			memcpy(destinationVar.v, &floatToCopy, destinationVar.m->SizeOf());
		}
		else if (sourceJSON.IsString())
		{
			*(char**)destinationVar.v = const_cast<char*>(sourceJSON.GetString());
		}
	}

	template<typename MetaVarType, typename ObjectType, typename DelType>
	static void PopulateDelegateDataInternal(MetaVarType& metaVar, ObjectType* object, SingleCastDelegate<DelType>& del, const char* functionName)
	{
		auto& fun = GetMetaFunctionByName<ObjectType>(functionName)->GetFunction();
		del.BindMemberFunction(object, fun);
	}

public:
	template<typename MetaVarType, typename DelType>
	static void PopulateDelegateData(MetaVarType& metaVar, SingleCastDelegate<DelType>& del, rapidjson::GenericArray<false, rapidjson::Value::ValueType>::PlainType& jsonData)
	{
		rapidjson::Value::MemberIterator varMemberIterator = jsonData.FindMember(metaVar.GetName());
		if (varMemberIterator != jsonData.MemberEnd())
		{
			const auto& jsonVar = varMemberIterator->value;
			const auto& jsonArray = jsonVar.GetArray();
#if RunInEngine
			if (strcmp((char*)"Engine", jsonArray[0].GetString()) == 0)
			{
				extern Engine* GetEngine();
				PopulateDelegateDataInternal(metaVar, GetEngine(), del, jsonArray[1].GetString());
			}
#endif
		}
	}

	template<typename MetaVarType, typename ...Args>
	static void PopulateVarData(MetaVarType& metaVar, rapidjson::GenericArray<false, rapidjson::Value::ValueType>::PlainType* jsonData, Args&&... args)
	{
		rapidjson::Value::MemberIterator varMemberIterator = jsonData->FindMember(metaVar.GetName());
		
		if (varMemberIterator != jsonData->MemberEnd())
		{
			const auto& jsonVar = varMemberIterator->value;

			Variable vars[]{ args... };
			const unsigned int varArraySize = (sizeof(vars) / sizeof(Variable));

			if (varArraySize > 1)
			{
				const auto& jsonArray = jsonVar.GetArray();

				for (unsigned int i = 0; i < varArraySize; ++i)
				{
					CopyVariableFromJSON(vars[i], jsonArray[i]);
				}
			}
			else
			{
				CopyVariableFromJSON(vars[0], jsonVar);
			}
		}
	}
};