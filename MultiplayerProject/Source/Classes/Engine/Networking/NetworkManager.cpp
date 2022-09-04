#include <iostream>

#include "Engine/Networking/NetworkManager.h"
#include "RakPeerInterface.h"
#include "Game/Entity.h"
#include <bitset>

#define MAX_CLIENTS 10
#define SERVER_PORT 60000
#define CLIENT_PORT 59999
#define MAX_BYTES_FOR_STREAM 1460

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

void NetworkManager::HandlePackets()
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
	
				Entity* entity = networkedObjectLinker.GetEntity(0);
				entity->posX = 500;
				entity->SerializeNetworkedVariables();
				peer->Send(&outputStream, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, packet->systemAddress, false);
			}
			break;
		case ID_NEW_INCOMING_CONNECTION:
			printf("A connection is incoming.\n");
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
		case ID_UPDATE_ENTITY:
		{
			RakNet::BitStream bsIn(packet->data, packet->length, false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	
			char* output = new char;
	
			while (bsIn.GetNumberOfBitsUsed() > bsIn.GetReadOffset())
			{
				unsigned int networkID = -1;
				bsIn.Read(output, sizeof(unsigned int));
				memcpy(&networkID, output, sizeof(unsigned int));
	
				unsigned int totalBytesToDeserialize = 0;
				bsIn.Read(output, sizeof(unsigned int));
				memcpy(&totalBytesToDeserialize, output, sizeof(unsigned int));
	
				Entity* entity = networkedObjectLinker.GetEntity(networkID);
	
				unsigned int entityBytesRead = 0;
				NetworkedVariableData nvd{0, 0, (AuthorityType)0};
	
	
				unsigned int sizeofOffset = 0;
				GetBitFieldSize(sizeofOffset, nvd.offset);
	
				unsigned int sizeofSize = 0;
				GetBitFieldSize(sizeofSize, nvd.size);
	
				unsigned int sizeofAuthorityType = 1;
	
				while (entityBytesRead < totalBytesToDeserialize)
				{
					unsigned int offset = 0;
					bsIn.Read(output, sizeofOffset);
					entityBytesRead += sizeofOffset;
					memcpy(&offset, output, sizeofOffset);
	
					unsigned int sizeofData = 0;
					bsIn.Read(output, sizeofSize);
					entityBytesRead += sizeofSize;
					memcpy(&sizeofData, output, sizeofSize);
	
					AuthorityType authorityType;
					bsIn.Read(output, sizeofAuthorityType);
					entityBytesRead += sizeofAuthorityType;
					memcpy(&authorityType, output, sizeofAuthorityType);
	
					bool variableAuthorityMatchesServerState =
						(authorityType == AuthorityType::Server && GetIsServer()) ||
						(authorityType == AuthorityType::OwningClient && !GetIsServer());
	
					bsIn.Read(output, sizeofData);
					entityBytesRead += sizeofData;
					if (variableAuthorityMatchesServerState)
					{
						continue;
					}
	
					char* dataToUpdate = (char*)entity + (offset);
					memcpy(dataToUpdate, output, sizeofData);
					//TODO: free dataToUpdate
					//free(dataToUpdate);
				}
			}
			//TODO: delete output for real
			//delete(output);
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

bool NetworkManager::CanSerializeNumberOfBytes(unsigned int size)
{
	return outputStream.GetNumberOfBytesUsed() + size < MAX_BYTES_FOR_STREAM;
}

void NetworkManager::SendSerializedData()
{
	if (outputStream.GetWriteOffset() <= 0)
	{
		return;
	}
	
	if (isServer)
	{
		assert(peer->Send(&outputStream, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true));
	}
	else
	{
		assert(peer->Send(&outputStream, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE, 0, serverGUID, false));
	}
	outputStream.Reset();
}

bool NetworkManager::Serialize(void* data, unsigned int size)
{
	if(!CanSerializeNumberOfBytes(size))
	{
		return false;
	}
	outputStream.Write((char*)data, size);
	return true;
}

void NetworkManager::Deserialize()
{
}

int NetworkManager::GenerateNewNetworkID()
{
	static int highestEntityID = -1;
	++highestEntityID;
	return highestEntityID;
}
