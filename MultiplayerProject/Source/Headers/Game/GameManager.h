#pragma once

#include "Engine/Managers/AssetManager.h"

#if ClientMode || InEditor
#include "Engine/GameInstances/ClientInstance.h"
#endif
#if ServerMode || InEditor
#include "Engine/GameInstances/ServerInstance.h"
#endif

#ifdef InEditor
#define GameInstanceCount 4
#else
#define GameInstanceCount 1
#endif

class RawPointerTracker;

extern class Engine* GetEngine();

class GameManager
{

public:
	GameManager() {}
	GameManager(GameManager const&) = delete;
	void operator=(GameManager const&) = delete;

#if ServerMode || InEditor
	ServerInstance serverInstance;
#endif
#if ClientMode || InEditor
	ClientInstance gameInstances[GameInstanceCount];
#endif
	GameInstance* currentGameInstance;

public:
	void Initialize();
	bool Update(float deltaTime);
	void Destroy();

	static NetworkManager* GetNetworkManager() 
	{
		GameInstance* gameInstance = GetCurrentGameInstance();
		assert(gameInstance && "Error: NetworkManager cannot be found.");
		return gameInstance ? &gameInstance->GetNetworkManager() : nullptr; 
	}

	static Renderer* GetRenderer();
	static Renderer* GetRendererFromWindowID(Uint32 windowID);
	static InputManager* GetInputManager() { return &Get().inputManager; }
	static EntityManager* GetEntityManager() 
	{ 
		GameInstance* gameInstance = GetCurrentGameInstance();
		assert(gameInstance && "Error: EntityManager cannot be found.");
		return gameInstance ? gameInstance->GetEntityManager() : nullptr; 
	}

	static TextureManager* GetTextureManager() 
	{ 
		GameInstance* gameInstance = GetCurrentGameInstance(); 
		return gameInstance ? gameInstance->GetTextureManager() : nullptr;
	}

	static RawPointerTracker* GetRawPointerTracker() 
	{ 
		GameInstance* gameInstance = GetCurrentGameInstance();
		return gameInstance ? gameInstance->GetRawPointerTracker() : nullptr;
	}

	static AssetManager* GetAssetManager()
	{
		return &Get().assetManager;
	}

	static GameManager& Get();
	static GameInstance* GetCurrentGameInstance() { return Get().currentGameInstance; }
	static void CloseGameInstance(int i);
	static void CloseGameInstances();
	static bool IsUpdatingFocusedWindow();

private:
	InputManager inputManager;
	AssetManager assetManager;
	void InitializeGameInstance(GameInstance& gameInstance);
	void UpdateGameInstance(float deltaTime, GameInstance& gameInstance);
};