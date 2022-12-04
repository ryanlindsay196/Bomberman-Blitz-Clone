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

void RPC::SendSerializedRpcData()
{
	NetworkManager* networkManager = GameManager::GetNetworkManager();
	networkManager->SendSerializedData(rpcStream);
}
