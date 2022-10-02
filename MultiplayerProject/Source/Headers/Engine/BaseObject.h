#pragma once
#include "Engine/Reflection/Reflection.h"
#include "Engine/Networking/NetworkEnums.h"

#include "mathfu/vector.h"

class BaseObject
{
public:
	CreateClassMetadata(BaseObject)

	virtual bool Initialize() = 0;
	void RegisterSelfAsNetworked();
	void RegisterNetworkedVariable(const AuthorityType authorityType, BaseObject::MetaVariable* metaVariable);
	unsigned int GetNetworkID() const { return networkID; }
	void GenerateNetworkID();
	bool HasValidNetworkID() const { return networkID != 0; }
private:
	unsigned int networkID = 0;
};