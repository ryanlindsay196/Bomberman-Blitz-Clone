#pragma once
#include <unordered_map>
#include "BitStream.h"
#include "Engine/Networking/NetworkEnums.h"

class BaseObject;

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

	void RegisterNetworkedObject(BaseObject* objectPtr);
	void RegisterNetworkedVariable(unsigned int networkID, void* networkedVariable, AuthorityType authorityType);
	int GenerateNewNetworkID();

	bool GetIsServer() { return isServer; }
	bool IsOutputStreamEmpty() { return outputStream.GetNumberOfBitsUsed() == 0; }
private:
	class RakNet::RakPeerInterface* peer;
	bool isServer;

	RakNet::BitStream outputStream;
	RakNet::BitStream inputStream;
};