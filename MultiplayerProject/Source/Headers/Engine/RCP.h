#include "BaseObject.h"
#include "Engine/Networking/NetworkedObjectLinker.h"

class RPC
{
	static void SerializeRpcData(Variable* vars, unsigned int varCount);
public:
	template<typename Class, typename MetaFunctionType, typename ...Args>
	static void SendRpc(Class* object, MetaFunctionType* metaFunction, Args&&... args)
	{
		if (!metaFunction || !object)
		{
			return;
		}

		unsigned int networkID = object->GetNetworkID();
		unsigned int metaFunctionID = metaFunction->GetMetaFunctionID();
		
		Variable vars[]{ networkID, metaFunctionID, args... };
		BaseObject* classInstance = NetworkedObjectLinker::GetInstance().GetBaseObject(networkID);

		MetaFunctionType* metaFunc = object->GetMetaFunctionByID(metaFunctionID);
		if (metaFunc)
		{
			metaFunc->Apply(classInstance, Variable(), &vars[2], 3);
		}

		SerializeRpcData(vars, sizeof(vars) / sizeof(Variable));
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

		//TODO: Make this buffer vary in size
		char tempOutputBuffer[8];

		unsigned int arg0;
		bsIn.Read(tempOutputBuffer, sizeof(arg0));
		memcpy(&arg0, tempOutputBuffer, sizeof(arg0));

		mathfu::Vector<float, 2> arg1;
		bsIn.Read(tempOutputBuffer, sizeof(arg1));
		memcpy(&arg1, tempOutputBuffer, sizeof(arg1));

		mathfu::Vector<int, 2> arg2;
		bsIn.Read(tempOutputBuffer, sizeof(arg2));
		memcpy(&arg2, tempOutputBuffer, sizeof(arg2));

		Variable vars[3]{ arg0, arg1, arg2 };

		if (metaFunc)
		{
			metaFunc->Apply(object, Variable(), &vars[0], 3);
		}
	}

	static void SendSerializedRpcData();
};