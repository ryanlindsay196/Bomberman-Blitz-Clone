#pragma once
#include <unordered_map>

#include "BitStream.h"
#include "MessageIdentifiers.h"

class Entity;

enum class AuthorityType
{
	OwningClient,
	Server
};

enum GameMessages
{
	ID_CREATE_ENTITY = ID_USER_PACKET_ENUM + 1,
	ID_UPDATE_ENTITY,
	ID_DESTROY_ENTITY
};


class NetworkedObjectLinker
{
public:
	Entity* GetEntity(unsigned int networkId)
	{
		return networkIdToEntityMap[networkId];
	}

	unsigned int GetNetworkId(Entity* entity)
	{
		return entityToNetworkIdMap[entity];
	}

private:
	friend class Entity;
	void AddEntity(Entity* inEntity, uint32_t inNetworkId)
	{
		networkIdToEntityMap[inNetworkId] = inEntity;
		entityToNetworkIdMap[inEntity] = inNetworkId;
	}

	void RemoveEntity(Entity* inEntity)
	{
		int networkId = entityToNetworkIdMap[inEntity];
		entityToNetworkIdMap.erase(inEntity);
		networkIdToEntityMap.erase(networkId);
	}

	void RemoveEntity(int inNetworkId)
	{
		Entity* entity = networkIdToEntityMap[inNetworkId];
		entityToNetworkIdMap.erase(entity);
		networkIdToEntityMap.erase(inNetworkId);
	}

	std::unordered_map<uint32_t, Entity*> networkIdToEntityMap;
	std::unordered_map<Entity*, uint32_t> entityToNetworkIdMap;
};

class NetworkManager
{
public:
	void Initialize();
	void HandlePackets();
	void Destroy();

	//Returns true if we have enough space to serialize "size" number of bytes.
	bool CanSerializeNumberOfBytes(unsigned int size);
	void SendSerializedData();
	bool Serialize(void* data, unsigned int size);
	void Deserialize();

	int GenerateNewNetworkID();

	NetworkedObjectLinker& GetNetworkedObjectLinker() { return networkedObjectLinker; }
	bool GetIsServer() { return isServer; }
	bool IsOutputStreamEmpty() { return outputStream.GetNumberOfBitsUsed() == 0; }
private:
	class RakNet::RakPeerInterface* peer;
	bool isServer;
	NetworkedObjectLinker networkedObjectLinker;

	RakNet::BitStream outputStream;
	RakNet::BitStream inputStream;
};