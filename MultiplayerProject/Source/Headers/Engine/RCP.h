#include "BaseObject.h"
#include "Engine/Networking/NetworkedObjectLinker.h"

class RPC
{
	static void SerializeRpcData(Variable* vars, unsigned int varCount);
public:
	template<typename Class, typename ...Args>
	static void SendRpc(Class* object, const char* functionName, Args&&... args)
	{
		if (!object)
		{
			return;
		}
		
		auto* metaFunction = Class::GetMetaFunctionByName(functionName);

		if (!metaFunction)
		{
			return;
		}
		
		unsigned int networkID = object->GetNetworkID();
		unsigned int metaFunctionID = metaFunction->GetMetaFunctionID();
		Variable vars[]{ networkID, metaFunctionID, args... };
		
		const unsigned int varsToSerializeCount = (sizeof(vars) / sizeof(Variable));
		const unsigned int argCount = varsToSerializeCount - 2;
		
		metaFunction->Apply(object, Variable(), &vars[2], argCount);
		
		SerializeRpcData(vars, argCount + 2);
	}

	static void ReceiveRpc(RakNet::BitStream& bsIn)
	{
		if (bsIn.GetNumberOfUnreadBits() <= 0)
		{
			return;
		}

		unsigned int networkID = 0;
		bsIn.Read((char*)&networkID, sizeof(networkID));
		unsigned int functionID = 0;
		bsIn.Read((char*)&functionID, sizeof(functionID));

		BaseObject* object = NetworkedObjectLinker::GetInstance().GetBaseObject(networkID);
		BaseObject::MetaFunction* metaFunc = object ? object->GetMetaFunctionByID(functionID) : nullptr;

		if (!metaFunc)
		{
			return;
		}

		std::vector<Variable> vars;
		vars.reserve(metaFunc->ArgCount());
		
		constexpr unsigned int maxArgCount = 8;
		constexpr unsigned int argBufferSize = 8;
		
		//We don't support functions with more arguments than maxArgCount
		assert(metaFunc->ArgCount() <= maxArgCount);

		char buffers[maxArgCount][argBufferSize];

		for (unsigned int i = 0; i < metaFunc->ArgCount(); ++i)
		{
			//We don't support parameters bigger than argBufferSize.
			assert(metaFunc->ArgType(i)->SizeOf() <= argBufferSize);

			bsIn.Read(buffers[i], metaFunc->ArgType(i)->SizeOf());
			vars.push_back(Variable(buffers[i]));
		}

		if (metaFunc)
		{
			metaFunc->Apply(object, Variable(), &vars[0], metaFunc->ArgCount());
		}
	}

	static void SendSerializedRpcData();
};