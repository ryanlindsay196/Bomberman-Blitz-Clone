#include "Game/GameManager.h"
#if !RunInEngine
#include <iostream>
#endif

void GameManager::Initialize()
{
	inputManager.Initialize();
#if RunInEngine || RunServerMode
	InitializeGameInstance(serverInstance);
#endif
#if RunInEngine || RunClientMode
	for (GameInstance& gameInstance : gameInstances)
	{
		InitializeGameInstance(gameInstance);
	}
#endif
}

void GameManager::InitializeGameInstance(GameInstance& gameInstance)
{
	currentGameInstance = &gameInstance;
	gameInstance.Initialize();
}

bool GameManager::Update(float deltaTime)
{
	inputManager.Update();

#if RunInEngine || RunServerMode
	UpdateGameInstance(deltaTime, serverInstance);
#endif
#if RunInEngine || RunClientMode
	for (GameInstance& gameInstance : gameInstances)
	{
		UpdateGameInstance(deltaTime, gameInstance);
	}
#endif RunServerMode

	return !inputManager.WantsToQuit();
}

void GameManager::UpdateGameInstance(float deltaTime, GameInstance & gameInstance)
{
	currentGameInstance = &gameInstance;
	gameInstance.Update(deltaTime);
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
#if RunClientMode || RunInEngine
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
	gameManager.serverInstance.Close();
	for (unsigned int i = 0; i < GameInstanceCount; ++i)
	{
		CloseGameInstance(i);
	}
}
