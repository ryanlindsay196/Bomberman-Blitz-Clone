#pragma once
#include "Engine/Reflection/Reflection.h"
#include "Engine/Networking/NetworkEnums.h"

class BaseObject
{
public:
	CreateClassMetadata(BaseObject)

	virtual bool Initialize(const char* imagePath, float inWidth, float inHeight, float inPosX, float inPosY) = 0;
	void RegisterSelfAsNetworked();
	void RegisterNetworkedVariable(const AuthorityType authorityType, BaseObject::MetaVariable* metaVariable);
	unsigned int GetNetworkID() const { return networkID; }
	void GenerateNetworkID();
	bool HasValidNetworkID() const { return networkID != 0; }
private:
	unsigned int networkID = 0;
};