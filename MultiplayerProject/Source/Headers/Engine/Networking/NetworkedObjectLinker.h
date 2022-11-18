#pragma once

#include <vector>
#include <unordered_map>
#include "Engine/Networking/NetworkEnums.h"
#include "Engine/BaseObject.h"

class BaseObject;

struct NetworkedMetaVariable
{
	NetworkedMetaVariable(BaseObject::MetaVariable* inMetaVariable, AuthorityType inAuthorityType, BaseObject::MetaFunction* inOnRepMetaFunction) :
		metaVariable(inMetaVariable),
		authorityType(inAuthorityType),
		onRepMetaFunction(inOnRepMetaFunction)
	{}
	BaseObject::MetaVariable* metaVariable;
	BaseObject::MetaFunction* onRepMetaFunction;
	AuthorityType authorityType;
	char data[8];
};

class NetworkedObjectLinker
{
public:
	void Initialize();

	BaseObject* GetBaseObject(unsigned int networkId) const;

	class NetworkedObjectProxy : public InstancedAutoLister<NetworkedObjectProxy>
	{
	public:
		NetworkedObjectProxy() {}
		NetworkedObjectProxy(BaseObject * inObject, NetworkedObjectProxy*& proxyHead);
		unsigned int GetNetworkID() const { return networkID; }
		BaseObject* GetNetworkedObject() const { return networkedObject; }

		void AddNetworkedVariable(BaseObject::MetaVariable* metaVariable, BaseObject::MetaFunction* onRepMetaFunction, AuthorityType authorityType);
		const std::vector<NetworkedMetaVariable>& GetNetworkedVariables() const { return networkedMetaVariables; }
	private:
		unsigned int networkID;
		BaseObject* networkedObject;
		std::vector<NetworkedMetaVariable> networkedMetaVariables;
	};

	void AddBaseObject(BaseObject* inObject);
	void RemoveBaseObject(unsigned int inNetworkId);

	NetworkedObjectProxy* GetNetworkedObjectProxy(unsigned int networkID) { 
		auto it = networkIdToNetworkObjectProxyMap.find(networkID);
		return it != networkIdToNetworkObjectProxyMap.end() ? &it->second : nullptr;
	}
	
	NetworkedObjectLinker::NetworkedObjectProxy* GetNetworkedObjectProxyHead() const { return headNetworkedObjectProxy; }

private:
	std::unordered_map<uint32_t, NetworkedObjectProxy> networkIdToNetworkObjectProxyMap;
	NetworkedObjectLinker::NetworkedObjectProxy* headNetworkedObjectProxy;
};