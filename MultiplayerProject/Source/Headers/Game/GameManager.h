#pragma once
#if RunClientMode || RunInEngine
#include "Engine/GameInstances/ClientInstance.h"
#endif
#if RunServerMode || RunInEngine
#include "Engine/GameInstances/ServerInstance.h"
#endif

#ifdef RunInEngine
#define GameInstanceCount 4
#else
#define GameInstanceCount 1
#endif

class GameManager
{
	GameManager() {}
	GameManager(GameManager const&) = delete;
	void operator=(GameManager const&) = delete;

#if RunServerMode || RunInEngine
	ServerInstance serverInstance;
#endif
#if RunClientMode || RunInEngine
	ClientInstance gameInstances[GameInstanceCount];
#endif
	GameInstance* currentGameInstance;

public:
	void Initialize();
	bool Update(float deltaTime);
	void Destroy();

	static NetworkManager& GetNetworkManager() { return GetCurrentGameInstance()->GetNetworkManager(); }
	static Renderer* GetRenderer() { return GetCurrentGameInstance()->GetRenderer(); }
	static Renderer* GetRendererFromWindowID(Uint32 windowID);
	static InputManager* GetInputManager() { return &Get().inputManager; }
	static EntityManager* GetEntityManager() { return GetCurrentGameInstance()->GetEntityManager(); }
	static TextureManager* GetTextureManager() { return GetCurrentGameInstance()->GetTextureManager(); }

	static GameManager& Get();
	static GameInstance* GetCurrentGameInstance() { return Get().currentGameInstance; }
	static void CloseGameInstance(int i);
	static void CloseGameInstances();

private:
	InputManager inputManager;
	void InitializeGameInstance(GameInstance& gameInstance);
	void UpdateGameInstance(float deltaTime, GameInstance& gameInstance);
};