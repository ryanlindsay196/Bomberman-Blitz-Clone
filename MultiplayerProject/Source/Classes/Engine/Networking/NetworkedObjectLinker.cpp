#include "Engine/Networking/NetworkedObjectLinker.h"
#include "Engine/BaseObject.h"

BaseObject* NetworkedObjectLinker::GetBaseObject(unsigned int networkId) const
{
	auto it = networkIdToNetworkObjectProxyMap.find(networkId);
	bool objectExistsForNetworkId = it != networkIdToNetworkObjectProxyMap.end();
	return objectExistsForNetworkId ? it->second.GetNetworkedObject() : nullptr;
}

void NetworkedObjectLinker::AddBaseObject(BaseObject * inObject)
{
	assert(inObject);

	unsigned int networkID = inObject->GetNetworkID();

	auto it = networkIdToNetworkObjectProxyMap.find(networkID);
	if (it != networkIdToNetworkObjectProxyMap.end())
	{
		return;
	}
	//We should only register a networked object once.
	networkIdToNetworkObjectProxyMap[networkID] = NetworkedObjectProxy(inObject);
}

void NetworkedObjectLinker::RemoveBaseObject(unsigned int inNetworkId)
{
	networkIdToNetworkObjectProxyMap.erase(inNetworkId);
}

NetworkedObjectLinker::NetworkedObjectProxy::NetworkedObjectProxy(BaseObject * inObject) :
	networkID(inObject->GetNetworkID()),
	networkedObject(inObject)
{}

void NetworkedObjectLinker::NetworkedObjectProxy::AddNetworkedVariable(BaseObject::MetaVariable* metaVariable, BaseObject::MetaFunction* onRepMetaFunction, AuthorityType authorityType)
{
	assert(metaVariable);

	networkedMetaVariables.push_back(NetworkedMetaVariable(metaVariable, authorityType, onRepMetaFunction));
}