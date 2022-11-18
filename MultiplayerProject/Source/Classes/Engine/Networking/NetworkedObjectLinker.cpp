#include "Engine/Networking/NetworkedObjectLinker.h"
#include "Engine/BaseObject.h"

void NetworkedObjectLinker::Initialize()
{
	headNetworkedObjectProxy = nullptr;
}

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
	networkIdToNetworkObjectProxyMap.emplace(std::piecewise_construct, std::forward_as_tuple(networkID), std::forward_as_tuple(inObject, headNetworkedObjectProxy));
}

void NetworkedObjectLinker::RemoveBaseObject(unsigned int inNetworkId)
{
	networkIdToNetworkObjectProxyMap.erase(inNetworkId);
}

NetworkedObjectLinker::NetworkedObjectProxy::NetworkedObjectProxy(BaseObject * inObject, NetworkedObjectProxy*& proxyHead) :
	InstancedAutoLister<NetworkedObjectProxy>(proxyHead),
	networkID(inObject->GetNetworkID()),
	networkedObject(inObject)
{}

void NetworkedObjectLinker::NetworkedObjectProxy::AddNetworkedVariable(BaseObject::MetaVariable* metaVariable, BaseObject::MetaFunction* onRepMetaFunction, AuthorityType authorityType)
{
	assert(metaVariable);

	networkedMetaVariables.push_back(NetworkedMetaVariable(metaVariable, authorityType, onRepMetaFunction));
}