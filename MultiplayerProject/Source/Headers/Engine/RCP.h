#include "BaseObject.h"
#include "Engine/Networking/NetworkedObjectLinker.h"
#include "Bitstream.h"

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

	static void ReceiveRpc(RakNet::BitStream& bsIn, const NetworkedObjectLinker& networkedObjectLinker);

	static void SendSerializedRpcData();
};