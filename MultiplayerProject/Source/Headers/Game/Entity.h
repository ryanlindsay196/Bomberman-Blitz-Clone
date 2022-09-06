#pragma once
#include "GameManager.h"
#include "Engine/Reflection/Reflection.h"

class SDL_Surface;
class SDL_Texture;

#define RegisterNetworkedVariable(myClass, variableToRegister, authorityType)\
networkedVariables.push_back({offsetof(myClass, variableToRegister), sizeof(variableToRegister), authorityType, "   "});\


#define RegisterNetworkedEntity() \
NetworkManager* networkManager = gameManager.GetNetworkManager();\
networkID = networkManager->GenerateNewNetworkID();\
networkManager->GetNetworkedObjectLinker().AddEntity(this, networkID);

struct NetworkedVariableData
{
	unsigned int offset : 16;
	unsigned int size : 8;
	AuthorityType authorityType : 8;
	char data[4];
};

class Entity
{
public:
	CreateClassReflectionMetadata(Entity);
	friend class EntityManager;
public:
	virtual ~Entity() {};
	virtual void Update(float deltaTime);
	virtual void Render();
	void Destroy();

protected:
	virtual bool Initialize(const char* imagePath, float inWidth, float inHeight, float inPosX, float inPosY);

private:
	void SerializeNetworkedVariables();
	void DeserializeNetworkedVariable(unsigned int offset, unsigned int size, char* dataSrc);
	int networkID;
	float width, height;

	float posX, posY;
	SDL_Texture* texture;

	std::vector<NetworkedVariableData> networkedVariables;
};