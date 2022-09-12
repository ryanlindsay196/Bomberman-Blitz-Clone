#pragma once

#include "NetworkEnums.h"
#include <vector>
#include <unordered_map>
#include "Engine/Networking/NetworkManager.h"

class BaseObject;

struct NetworkedMetaVariable
{
	BaseObject::MetaVariable* metaVariable;
	AuthorityType authorityType;
};

class NetworkedObjectLinker
{
	NetworkedObjectLinker() {};
	static NetworkedObjectLinker& GetInstance();

public:
	friend NetworkManager;

	BaseObject* GetBaseObject(unsigned int networkId);

	class NetworkedObjectProxy
	{
	public:
		NetworkedObjectProxy() {}
		NetworkedObjectProxy(BaseObject * inObject);
		unsigned int GetNetworkID() { return networkID; }
		BaseObject* GetNetworkedObject() { return networkedObject; }

		void AddNetworkedVariable(BaseObject::MetaVariable* metaVariable, AuthorityType authorityType);
	private:
		unsigned int networkID;
		BaseObject* networkedObject;
		std::vector<NetworkedMetaVariable> networkedVariables;
	};

	void AddBaseObject(BaseObject* inObject);

	void RemoveBaseObject(unsigned int inNetworkId);

	std::unordered_map<uint32_t, NetworkedObjectProxy> networkIdToBaseObjectMap;
};