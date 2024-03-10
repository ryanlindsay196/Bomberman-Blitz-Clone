#include "Game/GameManager.h"
#include "Engine/Engine.h"

void GameManager::Initialize()
{
	inputManager.Initialize();
#if InEditor || ServerMode
	InitializeGameInstance(serverInstance);
#endif
#if InEditor || ClientMode
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

#if InEditor || ServerMode
	UpdateGameInstance(deltaTime, serverInstance);
#endif
#if InEditor || ClientMode
	for (GameInstance& gameInstance : gameInstances)
	{
		UpdateGameInstance(deltaTime, gameInstance);
	}
#endif ServerMode
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
#if InEditor || ServerMode
	serverInstance.Destroy();
#endif
#if InEditor || ClientMode
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
#if InEditor
	if (Engine* engine = GetEngine())
	{
		return engine->GetRenderer();
	}
#endif

	return nullptr;
}

Renderer* GameManager::GetRendererFromWindowID(Uint32 windowID)
{ 
#if ServerMode
	return nullptr;
#endif
#if ClientMode || InEditor
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
#if ClientMode || InEditor
	GameManager& gameManager = GameManager::Get();
	gameManager.gameInstances[i].Close();
#elif ServerMode
	GameManager& gameManager = GameManager::Get();
	gameManager.serverInstance.Close();
#endif
}

void GameManager::CloseGameInstances()
{
	GameManager& gameManager = GameManager::Get();
#if InEditor || ServerMode
	gameManager.serverInstance.Close();
#endif
#if InEditor || ClientMode
	for (unsigned int i = 0; i < GameInstanceCount; ++i)
	{
		CloseGameInstance(i);
	}
#endif
}


bool GameManager::IsUpdatingFocusedWindow()
{
	static GameManager* gameManager = &GameManager::Get();

	Renderer* renderer = gameManager->GetRenderer();

	if (renderer)
	{
		Uint32 currentWindowFlags = renderer->GetWindowFlags();
		bool isUpdatingFocusedWindow = currentWindowFlags & SDL_WINDOW_INPUT_FOCUS;
		return isUpdatingFocusedWindow;
	}

	return false;
}
