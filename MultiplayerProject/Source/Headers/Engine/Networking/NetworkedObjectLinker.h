#pragma once

#include "NetworkEnums.h"

#include "Engine/BaseObject.h"

struct NetworkedReflectionVariable
{
	BaseObject::ReflectionVariable* reflectionVariable;
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

		void AddNetworkedVariable(void* reflectionVariable, AuthorityType authorityType);
	private:
		unsigned int networkID;
		BaseObject* networkedObject;
		std::vector<NetworkedReflectionVariable> networkedVariables;
	};

	void AddBaseObject(BaseObject* inObject);

	void RemoveBaseObject(unsigned int inNetworkId);

	std::unordered_map<uint32_t, NetworkedObjectProxy> networkIdToBaseObjectMap;
};