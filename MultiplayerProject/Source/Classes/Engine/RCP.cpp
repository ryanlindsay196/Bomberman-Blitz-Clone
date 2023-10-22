#include "Engine/RCP.h"
#include "Game/GameManager.h"
#include "Game/EntityManager.h"
#include "Engine/BaseObject.h"
#include "BitStream.h"


RakNet::BitStream rpcStream;

void RPC::SerializeRpcData(Variable* vars, unsigned int varCount)
{
	unsigned int sizeofRpcData = 0;
	
	RakNet::MessageID messID = (RakNet::MessageID)ID_CALL_RPC;

	//Is the stream empty?
	if (rpcStream.GetNumberOfBitsUsed() == 0)
	{
		sizeofRpcData += sizeof(messID);
	}

	for (unsigned int i = 0; i < varCount; ++i)
	{
		sizeofRpcData += vars[i].m->SizeOf();
	}

	NetworkManager* networkManager = GameManager::GetNetworkManager();

	if (!networkManager->CanSerializeNumberOfBytes(sizeofRpcData, rpcStream))
	{
		SendSerializedRpcData();
	}
	if (rpcStream.GetNumberOfBitsUsed() == 0)
	{
		networkManager->Serialize(&messID, sizeof(messID), rpcStream);
	}

	for (unsigned int i = 0; i < varCount; ++i)
	{
		networkManager->Serialize(vars[i].v, vars[i].m->SizeOf(), rpcStream);
	}
}

void RPC::ReceiveRpc(RakNet::BitStream & bsIn, const NetworkedObjectLinker & networkedObjectLinker)
{
	if (bsIn.GetNumberOfUnreadBits() <= 0)
	{
		return;
	}

	unsigned int networkID = 0;
	bsIn.Read((char*)&networkID, sizeof(networkID));
	unsigned int functionID = 0;
	bsIn.Read((char*)&functionID, sizeof(functionID));

	BaseObject* object = networkedObjectLinker.GetBaseObject(networkID);

	MetaFunction<BaseObject>* metaFunc = object ? object->GetClassMetaFunctionByID(functionID) : nullptr;

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

void RPC::SendSerializedRpcData()
{
	NetworkManager* networkManager = GameManager::GetNetworkManager();
	networkManager->SendSerializedData(rpcStream);
}
