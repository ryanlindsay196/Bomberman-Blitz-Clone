#include "Game/EntityManager.h"
#include "Game/GameManager.h"
#include "Engine/Networking/NetworkManager.h"
#include "Game/Player.h"
#include "Game/Entity.h"

EntityManager::EntityManager()
{
	InitializeNetworkID(GameManager::GetNetworkManager().GenerateNewNetworkID());
	RegisterSelfAsNetworked();
	CreateTemplatedClassFunctionMetadata(EntityManager, CreateEntity, <Player>);
	CreateClassFunctionMetadata(EntityManager, CreateController);
};

EntityManager * EntityManager::GetInstance()
{
	static EntityManager* instance = new EntityManager();
	return instance;
}


std::shared_ptr<CharacterController> EntityManager::CreateController(unsigned int characterNetworkID, unsigned int controllerNetworkID)
{

	std::shared_ptr<CharacterController> characterControllerPtr(new CharacterController);
	controllers.push_back(characterControllerPtr);
	characterControllerPtr->InitializeNetworkID(controllerNetworkID);
	characterControllerPtr->Initialize();

	Entity* entity = static_cast<Entity*>(GameManager::GetNetworkManager().GetObjectByNetworkID(characterNetworkID));
	if (entity != nullptr)
	{
		entity->SetController(characterControllerPtr);
	}

	return characterControllerPtr;
}

void EntityManager::UpdateEntities(float deltaTime)
{
	for (std::shared_ptr<Entity>& entity : entities)
	{
		entity->Update(deltaTime);
	}
}

void EntityManager::RenderEntities()
{
	for (std::shared_ptr<Entity>& entity : entities)
	{
		entity->Render();
	}
}
