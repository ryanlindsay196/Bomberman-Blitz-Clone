#include "Game/GameManager.h"

void GameManager::Initialize()
{
	bool isServer = true;
	for (GameInstance& gameInstance : gameInstances)
	{
		currentGameInstance = &gameInstance;
		gameInstance.Initialize(isServer);
		isServer = false;
	}
}

bool GameManager::Update(float deltaTime)
{
	for (GameInstance& gameInstance : gameInstances)
	{
		currentGameInstance = &gameInstance;
		gameInstance.Update(deltaTime);
	}

	return true;
}

void GameManager::Destroy()
{
	for (GameInstance& gameInstance : gameInstances)
	{
		gameInstance.Destroy();
	}
}

GameManager& GameManager::Get()
{
	static GameManager instance;
	return instance;
}

bool GameInstance::Initialize(bool isServer)
{
	networkManager.Initialize(isServer);
	renderer.Initialize(640, 480, false);
	uiManager.Initialize(&renderer);
	inputManager.Initialize();
	entityManager.Initialize();

	return true;
}

bool GameInstance::Update(float deltaTime)
{
	networkManager.Update(deltaTime);
	inputManager.Update();	
	renderer.Update(deltaTime);
	
	entityManager.UpdateEntities(deltaTime);
	
	networkManager.SerializeNetworkedObjects();
	
	entityManager.RenderEntities();

	uiManager.Update(&renderer);
	
	renderer.Render();
	return !inputManager.WantsToQuit();
}

void GameInstance::Destroy()
{
	networkManager.Destroy();
	renderer.Destroy();

	SDL_Quit();
}
