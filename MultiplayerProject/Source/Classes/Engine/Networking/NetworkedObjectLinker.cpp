#include "Engine/Networking/NetworkedObjectLinker.h"
#include "Engine/BaseObject.h"

NetworkedObjectLinker& NetworkedObjectLinker::GetInstance()
{
	static NetworkedObjectLinker instance;
	return instance;
}

BaseObject* NetworkedObjectLinker::GetBaseObject(unsigned int networkId)
{
	return networkIdToBaseObjectMap[networkId].GetNetworkedObject();
}

void NetworkedObjectLinker::AddBaseObject(BaseObject * inObject)
{
	unsigned int networkID = inObject->GetNetworkID();

	auto it = networkIdToBaseObjectMap.find(networkID);
	if (it != networkIdToBaseObjectMap.end())
	{
		return;
	}
	//We should only register a networked object once.
	networkIdToBaseObjectMap[networkID] = NetworkedObjectProxy(inObject);
}

void NetworkedObjectLinker::RemoveBaseObject(unsigned int inNetworkId)
{
	networkIdToBaseObjectMap.erase(inNetworkId);
}

NetworkedObjectLinker::NetworkedObjectProxy::NetworkedObjectProxy(BaseObject * inObject) :
	networkID(inObject->GetNetworkID()),
	networkedObject(inObject)
{}

void NetworkedObjectLinker::NetworkedObjectProxy::AddNetworkedVariable(BaseObject::ReflectionVariable* reflectionVariable, AuthorityType authorityType)
{
	networkedVariables.push_back(NetworkedReflectionVariable{ reflectionVariable, authorityType });
}