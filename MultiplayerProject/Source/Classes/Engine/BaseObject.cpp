#include "Engine/BaseObject.h"
#include "Game/GameManager.h"
#include "Engine/Networking/NetworkManager.h"


void BaseObject::RegisterSelfAsNetworked()
{
	if (!HasValidNetworkID())
	{
		GenerateNetworkID();
		GameManager::GetNetworkManager().RegisterNetworkedObject(this);
	}
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

void BaseObject::GenerateNetworkID()
{
	if (HasValidNetworkID())
	{
		return;
	}

	static NetworkManager& networkManager = GameManager::GetNetworkManager();
	networkID = networkManager.GenerateNewNetworkID();
}


