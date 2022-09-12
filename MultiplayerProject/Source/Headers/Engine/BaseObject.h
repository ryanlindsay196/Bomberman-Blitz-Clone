#pragma once
#include "Engine/Reflection/Reflection.h"
#include "Engine/Networking/NetworkManager.h"

class BaseObject
{
public:
	CreateClassReflectionMetadata(BaseObject)

	virtual bool Initialize(const char* imagePath, float inWidth, float inHeight, float inPosX, float inPosY) = 0;
	void RegisterSelfAsNetworked();
	void RegisterNetworkedVariable(const AuthorityType authorityType, void* reflectionVariable);
	unsigned int GetNetworkID() { return networkID; }
	void GenerateNetworkID();
	bool HasValidNetworkID() { return networkID != 0; }
private:
	unsigned int networkID = 0;
};