#pragma once

#include <vector>
#include <unordered_map>
#include "Engine/Networking/NetworkEnums.h"
#include "Engine/Networking/NetworkManager.h"

class BaseObject;

struct NetworkedMetaVariable
{
	NetworkedMetaVariable(BaseObject::MetaVariable* inMetaVariable, AuthorityType inAuthorityType) :
		metaVariable(inMetaVariable),
		authorityType(inAuthorityType)
	{}
	BaseObject::MetaVariable* metaVariable;
	AuthorityType authorityType;
	char data[4];
};

class NetworkedObjectLinker
{
	NetworkedObjectLinker() {};
	static NetworkedObjectLinker& GetInstance();

public:
	friend NetworkManager;

	BaseObject* GetBaseObject(unsigned int networkId);

	class NetworkedObjectProxy : public AutoLister<NetworkedObjectProxy>
	{
	public:
		NetworkedObjectProxy() {}
		NetworkedObjectProxy(BaseObject * inObject);
		unsigned int GetNetworkID() const { return networkID; }
		BaseObject* GetNetworkedObject() const { return networkedObject; }

		void AddNetworkedVariable(BaseObject::MetaVariable* metaVariable, AuthorityType authorityType);
		const std::vector<NetworkedMetaVariable>& GetNetworkedVariables() const { return networkedMetaVariables; }
	private:
		unsigned int networkID;
		BaseObject* networkedObject;
		std::vector<NetworkedMetaVariable> networkedMetaVariables;
	};

	void AddBaseObject(BaseObject* inObject);
	void RemoveBaseObject(unsigned int inNetworkId);

	NetworkedObjectProxy* GetNetworkedObjectProxy(unsigned int networkID) { return &networkIdToNetworkObjectProxyMap[networkID]; }

private:
	std::unordered_map<uint32_t, NetworkedObjectProxy> networkIdToNetworkObjectProxyMap;
};