#include "Game/EntityManager.h"
#include "Game/GameManager.h"
#include "Engine/Networking/NetworkManager.h"
#include "Game/Player.h"
#include "Game/Entity.h"

CreateClassMetadata(EntityManager);

bool EntityManager::Initialize()
{
	NetworkManager* networkManager = GameManager::GetNetworkManager();

	InitializeNetworkID(networkManager->GenerateNewNetworkID());
	RegisterSelfAsNetworked();
	CreateTemplatedClassFunctionMetadata(EntityManager, CreateEntity, <Player>);	

	CreateClassFunctionMetadata(EntityManager, CreateController);

	return true;
};

std::shared_ptr<CharacterController> EntityManager::CreateController(unsigned int characterNetworkID, unsigned int controllerNetworkID, unsigned int owningClientID)
{

	std::shared_ptr<CharacterController> characterControllerPtr(new CharacterController);
	controllers.push_back(characterControllerPtr);
	characterControllerPtr->InitializeNetworkID(controllerNetworkID);
	characterControllerPtr->InitializeOwningClientID(owningClientID);
	characterControllerPtr->Initialize();

	NetworkManager* networkManager = GameManager::GetNetworkManager();

	Entity* entity = static_cast<Entity*>(networkManager->GetObjectByNetworkID(characterNetworkID));
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
