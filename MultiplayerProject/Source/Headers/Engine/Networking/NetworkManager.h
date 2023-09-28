#pragma once
#include <unordered_map>
#include "BitStream.h"
#include "Engine/Networking/NetworkEnums.h"
#include "Engine/BaseObject.h"
#include "Engine/Reflection/Reflection.h"
#include "Engine/Networking/NetworkedObjectLinker.h"

class GameMode;

class NetworkManager
{
public:
	void Initialize(bool inIsServer);
	void Update(float deltaTime);
	void Destroy();

	//Returns true if we have enough space to serialize "size" number of bytes.
	bool CanSerializeNumberOfBytes(unsigned int size, const RakNet::BitStream& bsIn) const;
	void SendSerializedData(RakNet::BitStream& bitStream);
	void SerializeNetworkedObjects();

	void RegisterNetworkedObject(BaseObject* objectPtr);
	void RegisterNetworkedVariable(unsigned int networkID, ClassMetaData<BaseObject>::MetaVariable* networkedVariable, MetaFunction<BaseObject>* onRepMetaFunction, AuthorityType authorityType);
	unsigned int GenerateNewNetworkID();
	unsigned int GenerateClientID();

	bool GetIsServer() const { return isServer; }
	unsigned int GetClientID() const { return clientID; }
	BaseObject* GetObjectByNetworkID(unsigned int networkID);
	bool IsOutputStreamEmpty() const { return outputStream.GetNumberOfBitsUsed() == 0; }
	bool Serialize(void* data, unsigned int size, RakNet::BitStream& bitStream);

private:
	bool IsClientIDValidForPacket(const RakNet::Packet* packetToCheck, unsigned int clientIDToCheck) const;
	void HandleReceivedPackets();

	class RakNet::RakPeerInterface* peer;
	bool isServer;
	unsigned int clientID;
	int highestNetworkID;
	std::vector<std::pair<unsigned int, RakNet::RakNetGUID>> clientIDToGuidVector;
	RakNet::BitStream outputStream;
	RakNet::BitStream inputStream;

	NetworkedObjectLinker networkedObjectLinker;
	GameMode* gameMode;
};