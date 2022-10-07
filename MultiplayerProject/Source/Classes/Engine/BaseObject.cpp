#include "Engine/BaseObject.h"
#include "Game/GameManager.h"
#include "Engine/Networking/NetworkManager.h"


void BaseObject::InitializeNetworkID(unsigned int newNetworkID)
{
	if (!HasValidNetworkID())
	{
		networkID = newNetworkID;
	}
}

void BaseObject::RegisterSelfAsNetworked()
{
	GameManager::GetNetworkManager().RegisterNetworkedObject(this);
}

void BaseObject::RegisterNetworkedVariable(const AuthorityType authorityType, BaseObject::MetaVariable* metaVariable)
{
	if (!metaVariable)
	{
		return;
	}

	static NetworkManager& networkManager = GameManager::GetNetworkManager();
	networkManager.RegisterNetworkedVariable(GetNetworkID(), metaVariable, authorityType);
}
