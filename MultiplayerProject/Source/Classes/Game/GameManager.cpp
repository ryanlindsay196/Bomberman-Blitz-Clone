#include "Game/GameManager.h"
#include "Engine/Engine.h"

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
	currentGameInstance = nullptr;

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

Renderer * GameManager::GetRenderer()
{
	if (GameInstance* gameInstance = GetCurrentGameInstance())
	{
		return gameInstance->GetRenderer();
	}
#if RunInEngine
	if (Engine* engine = GetEngine())
	{
		return engine->GetRenderer();
	}
#endif

	return nullptr;
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
#if RunInEngine || RunServerMode
	gameManager.serverInstance.Close();
#endif
#if RunInEngine || RunClientMode
	for (unsigned int i = 0; i < GameInstanceCount; ++i)
	{
		CloseGameInstance(i);
	}
#endif
}
