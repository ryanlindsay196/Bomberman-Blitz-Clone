#pragma once
#include <unordered_map>
#include "BitStream.h"
#include "Engine/Networking/NetworkEnums.h"
#include "Engine/BaseObject.h"

class NetworkManager
{
public:
	void Initialize();
	void HandlePackets();
	void Destroy();

	//Returns true if we have enough space to serialize "size" number of bytes.
	bool CanSerializeNumberOfBytes(unsigned int size) const;
	void SendSerializedData();
	bool Serialize(BaseObject::MetaVariable* data, unsigned int size);

	void RegisterNetworkedObject(BaseObject* objectPtr);
	void RegisterNetworkedVariable(unsigned int networkID, BaseObject::MetaVariable* networkedVariable, AuthorityType authorityType);
	int GenerateNewNetworkID();

	bool GetIsServer() const { return isServer; }
	bool IsOutputStreamEmpty() const { return outputStream.GetNumberOfBitsUsed() == 0; }
private:
	class RakNet::RakPeerInterface* peer;
	bool isServer;

	RakNet::BitStream outputStream;
	RakNet::BitStream inputStream;
};