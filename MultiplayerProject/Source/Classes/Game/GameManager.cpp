#include "Game/GameManager.h"
#if !RunInEngine
#include <iostream>
#endif

void GameManager::Initialize()
{
	inputManager.Initialize();
#if !RunInEngine
	bool isServer = false;
	std::cout << "Client (C) or server (S)?" << std::endl;
	char str[512];
	std::cin >> str;
	if (str[0] == 'S' || str[0] == 's')
	{
		isServer = true;
	}
#else
	bool isServer = true;
#endif
	for (GameInstance& gameInstance : gameInstances)
	{
		currentGameInstance = &gameInstance;
		gameInstance.Initialize(isServer);
		isServer = false;
	}
}

bool GameManager::Update(float deltaTime)
{
	inputManager.Update();

	for (GameInstance& gameInstance : gameInstances)
	{
		currentGameInstance = &gameInstance;
		gameInstance.Update(deltaTime);
	}

	return !inputManager.WantsToQuit();
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
	entityManager.Initialize();

	return true;
}

void GameInstance::Update(float deltaTime)
{
	networkManager.Update(deltaTime);
	renderer.Update(deltaTime);
	
	entityManager.UpdateEntities(deltaTime);
	
	networkManager.SerializeNetworkedObjects();
	
	entityManager.RenderEntities();

	uiManager.Update(&renderer);
	
	renderer.Render();
}

void GameInstance::Destroy()
{
	networkManager.Destroy();
	renderer.Destroy();

	SDL_Quit();
}
