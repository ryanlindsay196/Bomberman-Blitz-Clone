#include "Game/GameManager.h"
#if !RunInEngine
#include <iostream>
#endif

void GameManager::Initialize()
{
	inputManager.Initialize();
#if RunInEngine || RunServerMode
	currentGameInstance = &serverInstance;
	serverInstance.Initialize();
#endif
#if RunInEngine || RunClientMode
	for (GameInstance& gameInstance : gameInstances)
	{
		currentGameInstance = &gameInstance;
		gameInstance.Initialize();
	}
#endif
}

bool GameManager::Update(float deltaTime)
{
	inputManager.Update();

#if RunInEngine || RunServerMode
	currentGameInstance = &serverInstance;
	serverInstance.Update(deltaTime);
#endif
#if RunInEngine || RunClientMode
	for (GameInstance& gameInstance : gameInstances)
	{
		currentGameInstance = &gameInstance;
		gameInstance.Update(deltaTime);
	}
#endif RunServerMode

	return !inputManager.WantsToQuit();
}

void GameManager::Destroy()
{
#if RunInEngine || RunServerMode
	serverInstance.Destroy();
#endif
#if RunInEngine || RunClientMode
	for (GameInstance& gameInstance : gameInstances)
	{
		gameInstance.Destroy();
	}
#endif
}

Renderer* GameManager::GetRendererFromWindowID(Uint32 windowID)
{ 
#if RunServerMode
	return nullptr;
#endif
#if RunClientMode || RunInEngine
	for (GameInstance& instance : Get().gameInstances)
	{
		if (instance.GetRenderer()->GetWindowID() == windowID)
		{
			return instance.GetRenderer();
		}
	}
	return nullptr;
#endif
}

GameManager& GameManager::Get()
{
	static GameManager instance;
	return instance;
}

void GameManager::CloseGameInstance(int i)
{
#if RunClientMode || RunInEditor
	GameManager& gameManager = GameManager::Get();
	gameManager.gameInstances[i].Close();
#elif RunServerMode
	GameManager& gameManager = GameManager::Get();
	gameManager.serverInstance.Close();
#endif
}

void GameManager::CloseGameInstances()
{
	GameManager& gameManager = GameManager::Get();
	for (unsigned int i = 0; i < GameInstanceCount; ++i)
	{
		CloseGameInstance(i);
	}
}
