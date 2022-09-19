#pragma once
#include <unordered_map>
#include "BitStream.h"
#include "Engine/Networking/NetworkEnums.h"
#include "Engine/BaseObject.h"

class NetworkManager
{
public:
	void Initialize();
	void HandleReceivedPackets();
	void Destroy();

	//Returns true if we have enough space to serialize "size" number of bytes.
	bool CanSerializeNumberOfBytes(unsigned int size) const;
	void SendSerializedData();
	void SerializeNetworkedObjects();

	void RegisterNetworkedObject(BaseObject* objectPtr);
	void RegisterNetworkedVariable(unsigned int networkID, BaseObject::MetaVariable* networkedVariable, AuthorityType authorityType);
	int GenerateNewNetworkID();

	bool GetIsServer() const { return isServer; }
	bool IsOutputStreamEmpty() const { return outputStream.GetNumberOfBitsUsed() == 0; }
private:
	bool Serialize(void* data, unsigned int size);
	class RakNet::RakPeerInterface* peer;
	bool isServer;

	RakNet::BitStream outputStream;
	RakNet::BitStream inputStream;
};