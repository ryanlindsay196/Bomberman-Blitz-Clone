#include <iostream>

#include "Engine/Networking/NetworkManager.h"
#include "Engine/Networking/NetworkedObjectLinker.h"
#include "Engine/GameMode/GameMode.h"
#include "Engine/RCP.h"
#include "RakPeerInterface.h"
#include <bitset>

#define MAX_CLIENTS 10
#define SERVER_PORT 60000
#define CLIENT_PORT 59999
#define MAX_BYTES_FOR_STREAM 1460
#define SERVER_ID 0

#define GetBitFieldSize(fieldSize, field)\
{\
field = 0;\
field = ~field;\
std::bitset<sizeof(unsigned int) * 8> fieldBitset(field);\
fieldSize = BITS_TO_BYTES(fieldBitset.count());\
}

static RakNet::AddressOrGUID serverGUID = RakNet::UNASSIGNED_RAKNET_GUID;

void NetworkManager::Initialize()
{
	highestNetworkID = 0;

	char str[512];
	peer = RakNet::RakPeerInterface::GetInstance();
	
	printf("(C) or (S)erver?\n");
	std::cin >> str;
	if (
		//true
		(str[0] == 'c') || (str[0] == 'C')
		)
	{
		RakNet::SocketDescriptor sd;
		peer->Startup(1, &sd, 1);
		isServer = false;
	}
	else
	{
		RakNet::SocketDescriptor sd(SERVER_PORT, 0);
		peer->Startup(MAX_CLIENTS, &sd, 1);
		isServer = true;
	}
	
	if (isServer)
	{
		std::cout << "Starting the server." << std::endl;
		//We need to let the server accept incoming connections from the clients
		peer->SetMaximumIncomingConnections(MAX_CLIENTS);
	}
	else
	{
		std::cout << "Enter server IP or hit enter for 127.0.0.1" << std::endl;
		//std::cin >> str;
		//if (str[0] == 0 || str[0] == '\0')
		{
			strcpy_s(str, "127.0.0.1");
		}
		std::cout << "Starting the client." << std::endl;
		peer->Connect(str, SERVER_PORT, 0, 0);
	}
}

void NetworkManager::Update(float deltaTime)
{
	HandleReceivedPackets();
	if (gameMode)
	{
		gameMode->Update(deltaTime);
	}
	RPC::SendSerializedRpcData();
}

void NetworkManager::HandleReceivedPackets()
{
	assert(peer);

	for (RakNet::Packet* packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
	{
		switch (packet->data[0])
		{
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			std::cout << "Another client has disconnected" << std::endl;
			break;
		case ID_REMOTE_CONNECTION_LOST:
			printf("Another client has lost the connection.\n");
			break;
		case ID_REMOTE_NEW_INCOMING_CONNECTION:
			printf("Another client has connected.\n");
			break;
		case ID_CONNECTION_REQUEST_ACCEPTED:
			printf("Our connection request has been accepted.\n");
			{
				serverGUID = packet->guid;
			}
			break;
		case ID_NEW_INCOMING_CONNECTION:
			printf("A connection is incoming.\n");
			{
				RakNet::MessageID messID = (RakNet::MessageID)ID_INITIALIZE_NEW_CLIENT;
				Serialize(&messID, sizeof(messID), outputStream);

				unsigned int clientID = GenerateClientID();
				Serialize((void*)&clientID, sizeof(clientID), outputStream);
				clientIDToGuidVector.push_back(std::pair<unsigned int, RakNet::RakNetGUID>(clientID, packet->guid));

				peer->Send(&outputStream, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE, 0, packet->guid, false);
				outputStream.Reset();

				gameMode = new GameMode();
				gameMode->StartGameMode(new MatchSetupPhase());
			}
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			printf("The server is full.\n");
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			if (isServer) {
				printf("A client has disconnected.\n");
			}
			else {
				printf("We have been disconnected.\n");
			}
			break;
		case ID_CONNECTION_LOST:
			if (isServer) {
				printf("A client lost the connection.\n");
			}
			else {
				printf("Connection lost.\n");
			}
			break;
		case ID_INITIALIZE_NEW_CLIENT:
			{
				RakNet::BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				char tempOutputBuffer[4];
				bsIn.Read(tempOutputBuffer, sizeof(clientID));
				memcpy(&clientID, tempOutputBuffer, sizeof(clientID));
				printf("Initializing client!\n");

				clientIDToGuidVector.push_back(std::pair<unsigned int, RakNet::RakNetGUID>(SERVER_ID, packet->guid));
			}
			break;
		case ID_UPDATE_ENTITY:
			{
				RakNet::BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

				char tempOutputBuffer[8];

				unsigned int senderClientID = 0;
				bsIn.Read(tempOutputBuffer, sizeof(senderClientID));
				memcpy(&senderClientID, tempOutputBuffer, sizeof(senderClientID));

				if (!IsClientIDValidForPacket(packet, senderClientID))
				{
					continue;
				}

				while (bsIn.GetNumberOfBitsUsed() > bsIn.GetReadOffset())
				{
					unsigned int networkID = -1;
					bsIn.Read(tempOutputBuffer, sizeof(unsigned int));
					memcpy(&networkID, tempOutputBuffer, sizeof(unsigned int));
	
					unsigned int totalBytesToDeserialize = 0;
					bsIn.Read(tempOutputBuffer, sizeof(unsigned int));
					memcpy(&totalBytesToDeserialize, tempOutputBuffer, sizeof(unsigned int));
					
					auto* proxy = networkedObjectLinker.GetNetworkedObjectProxy(networkID);
					unsigned int entityBytesRead = 0;
					//Deserialize all bytes for a particular entity
					while (entityBytesRead < totalBytesToDeserialize && proxy != nullptr)
					{//TODO: Do something else with the proxy inside the while statement above
						unsigned int networkedVariableIndex = -1;
						bsIn.Read(tempOutputBuffer, sizeof(networkedVariableIndex));
						entityBytesRead += sizeof(networkedVariableIndex);
						memcpy(&networkedVariableIndex, tempOutputBuffer, sizeof(networkedVariableIndex));

						const std::vector<NetworkedMetaVariable>& networkedMetaVariables = proxy->GetNetworkedVariables();

						const NetworkedMetaVariable* networkedMetaVariable = networkedVariableIndex < networkedMetaVariables.size() ? 
							&networkedMetaVariables[networkedVariableIndex] : nullptr;

						AuthorityType authorityType = networkedMetaVariable ? networkedMetaVariable->authorityType : AuthorityType::Server;

						bool senderHasAuthority = (authorityType == AuthorityType::OwningClient && proxy->GetNetworkedObject()->GetOwningClientID() == senderClientID) ||
							(proxy->GetNetworkedObject()->GetOwningClientID() != clientID && senderClientID == SERVER_ID);
							//TODO: proxy->GetOwningClientID() != clientID will need to change for implementing dead reckoning and client-side prediction

						unsigned int sizeofVariableData = networkedMetaVariable ? networkedMetaVariable->metaVariable->GetSize() : 0;
						bsIn.Read(tempOutputBuffer, sizeofVariableData);
						entityBytesRead += sizeofVariableData;

						//If we have authority over this variable, then we shouldn't update based on packets received from clients
						if (senderHasAuthority && networkedMetaVariable != nullptr)
						{
							char* dataToUpdate = (char*)proxy->GetNetworkedObject() + networkedMetaVariable->metaVariable->GetOffset();

							if (networkedMetaVariable->onRepMetaFunction)
							{
								Variable vars[3]{ tempOutputBuffer, *dataToUpdate, dataToUpdate };
								networkedMetaVariable->onRepMetaFunction->Apply(proxy->GetNetworkedObject(), Variable(), vars, 3);
							}
							else
							{
								memcpy(dataToUpdate, tempOutputBuffer, sizeofVariableData);
							}
						}
					}
				}
			}
			break;
		case ID_CALL_RPC:
			{
				RakNet::BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				while (bsIn.GetNumberOfBitsUsed() > bsIn.GetReadOffset())
				{
					RPC::ReceiveRpc(bsIn, networkedObjectLinker);
				}
			}
			break;
		default:
			printf("Message with identifier %i has arrived.\n", packet->data[0]);
			break;
		}
	}
}

void NetworkManager::Destroy()
{
	RakNet::RakPeerInterface::DestroyInstance(peer);
}

bool NetworkManager::CanSerializeNumberOfBytes(unsigned int size, const RakNet::BitStream& bsIn) const
{
	return bsIn.GetNumberOfBytesUsed() + size < MAX_BYTES_FOR_STREAM;
}

void NetworkManager::SendSerializedData(RakNet::BitStream& bitStream)
{
	if (bitStream.GetWriteOffset() <= 0)
	{
		return;
	}
	
	if (isServer)
	{
		assert(peer->Send(&bitStream, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true));
	}
	else
	{
		assert(peer->Send(&bitStream, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE, 0, serverGUID, false));
	}
	bitStream.Reset();
}

void PrepareDataToSerialize(const NetworkedObjectLinker::NetworkedObjectProxy* proxy, bool isServer, unsigned int clientID,
	std::vector<unsigned int>& outUpdatedVariableIDs, unsigned int& outProxyDataSizeInBytes)
{
	const std::vector<NetworkedMetaVariable>& networkedMetaVariables = proxy->GetNetworkedVariables();
	for (unsigned int i = 0; i < networkedMetaVariables.size(); ++i)
	{
		const NetworkedMetaVariable& networkedMetaVar = networkedMetaVariables[i];
		bool localMachineHasAuthority = isServer ||
			(proxy->GetNetworkedObject()->GetOwningClientID() == clientID && networkedMetaVar.authorityType == AuthorityType::OwningClient);

		const MetaType& desiredMetaType = networkedMetaVar.metaVariable->GetMetaType();
		void* dataToSerialize = (char*)proxy->GetNetworkedObject() + networkedMetaVar.metaVariable->GetOffset();
		bool wasVariableUpdatedLocally = memcmp(dataToSerialize, (void*)networkedMetaVar.data, networkedMetaVar.metaVariable->GetSize());

		if (localMachineHasAuthority && wasVariableUpdatedLocally)
		{
			outUpdatedVariableIDs.push_back(i);
			outProxyDataSizeInBytes += sizeof(unsigned int);//Adding this here because we want to serialize the index.
			outProxyDataSizeInBytes += networkedMetaVar.metaVariable->GetSize();
		}
	}
}

void NetworkManager::SerializeNetworkedObjects()
{
	for (const NetworkedObjectLinker::NetworkedObjectProxy* proxy = NetworkedObjectLinker::NetworkedObjectProxy::Head(); proxy; proxy = proxy->Next())
	{
		unsigned int proxyDataSizeInBytes = 0;
		std::vector<unsigned int> replicatedVariableIndices;
		PrepareDataToSerialize(proxy, GetIsServer(), clientID, replicatedVariableIndices, proxyDataSizeInBytes);

		if (proxyDataSizeInBytes <= 0)
		{
			continue;
		}

		unsigned int networkID = proxy->GetNetworkID();
		if (!CanSerializeNumberOfBytes(sizeof(networkID) + sizeof(proxyDataSizeInBytes) + proxyDataSizeInBytes, outputStream))
		{
			SendSerializedData(outputStream);
		}

		if (IsOutputStreamEmpty())
		{
			RakNet::MessageID messID = (RakNet::MessageID)ID_UPDATE_ENTITY;
			Serialize(&messID, sizeof(messID), outputStream);
			Serialize(&clientID, sizeof(clientID), outputStream);
		}


		Serialize(&networkID, sizeof(networkID), outputStream);
		Serialize(&proxyDataSizeInBytes, sizeof(proxyDataSizeInBytes), outputStream);
		
		const std::vector<NetworkedMetaVariable>& networkedVars = proxy->GetNetworkedVariables();

		for(unsigned int i = 0; i < replicatedVariableIndices.size(); ++i)
		{
			Serialize((void*)&replicatedVariableIndices[i], sizeof(i), outputStream);//Serializing networkVariableIndex
			
			const NetworkedMetaVariable& varToReplicate = networkedVars[replicatedVariableIndices[i]];
			void* dataToSerialize = (char*)proxy->GetNetworkedObject() + varToReplicate.metaVariable->GetOffset();
			Serialize(dataToSerialize, varToReplicate.metaVariable->GetSize(), outputStream);

			//Make sure we have enough room to cache this variable.
			assert(sizeof(varToReplicate.data) > sizeof(varToReplicate.metaVariable->GetSize()));

			//Updating cached data
			memcpy((void*)&varToReplicate.data, dataToSerialize, varToReplicate.metaVariable->GetSize());
		}
	}
	SendSerializedData(outputStream);
}

BaseObject * NetworkManager::GetObjectByNetworkID(unsigned int networkID)
{
	return networkedObjectLinker.GetBaseObject(networkID);
}

bool NetworkManager::Serialize(void* data, unsigned int size, RakNet::BitStream& bitStream)
{
	if(!CanSerializeNumberOfBytes(size, bitStream))
	{
		return false;
	}
	bitStream.Write((char*)data, size);
	return true;
}

bool NetworkManager::IsClientIDValidForPacket(const RakNet::Packet * packetToCheck, unsigned int clientIDToCheck) const
{
	for (std::pair<unsigned int, RakNet::RakNetGUID> pair : clientIDToGuidVector)
	{
		if (packetToCheck->guid == pair.second && clientIDToCheck == pair.first)
		{
			return true;
		}
	}
	return false;
}

void NetworkManager::RegisterNetworkedObject(BaseObject * objectPtr)
{
	assert(objectPtr);

	networkedObjectLinker.AddBaseObject(objectPtr);
}

void NetworkManager::RegisterNetworkedVariable(unsigned int networkID, BaseObject::MetaVariable* networkedVariable, BaseObject::MetaFunction* onRepMetaFunction, AuthorityType authorityType)
{
	auto* proxy = networkedObjectLinker.GetNetworkedObjectProxy(networkID);
	if (!proxy)
	{
		return;
	}

	proxy->AddNetworkedVariable(networkedVariable, onRepMetaFunction, authorityType);
}

unsigned int NetworkManager::GenerateNewNetworkID()
{
	++highestNetworkID;
	return highestNetworkID;
}

unsigned int NetworkManager::GenerateClientID()
{
	static unsigned int clientID = 0;
	++clientID;
	return clientID;
}
