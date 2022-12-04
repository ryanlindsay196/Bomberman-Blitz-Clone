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

void BaseObject::InitializeOwningClientID(unsigned int newOwningClientID)
{
	if (!HasValidOwningClientID())
	{
		owningClientID = newOwningClientID;
	}
}

void BaseObject::RegisterSelfAsNetworked()
{
	NetworkManager* networkManager = GameManager::GetNetworkManager();
	networkManager->RegisterNetworkedObject(this);
}

void BaseObject::RegisterNetworkedVariable(const AuthorityType authorityType, BaseObject::MetaVariable* metaVariable, MetaFunction* onRepMetaFunction)
{
	if (!metaVariable)
	{
		return;
	}

	NetworkManager* networkManager = GameManager::GetNetworkManager();
	networkManager->RegisterNetworkedVariable(GetNetworkID(), metaVariable, onRepMetaFunction, authorityType);
}
