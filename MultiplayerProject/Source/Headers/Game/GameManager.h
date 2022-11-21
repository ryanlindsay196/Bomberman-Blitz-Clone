#pragma once
#include "Engine/Rendering/Renderer.h"
#include "Engine/InputManager.h"
#include "Engine/Networking/NetworkManager.h"
#include "Engine/Managers/TextureManager.h"
#include "Engine/UI/UIManager.h"
#include "Game/EntityManager.h"

#ifdef RunInEngine
#define GameInstanceCount 4
#else
#define GameInstanceCount 1
#endif

class GameInstance
{
public:
	bool Initialize(bool isServer);
	void Update(float deltaTime);
	void Open();
	void Close();
	void Destroy();

	NetworkManager& GetNetworkManager() { return networkManager; }
	Renderer* GetRenderer() { return &renderer; }
	EntityManager* GetEntityManager() { return &entityManager; }
	TextureManager* GetTextureManager() { return &textureManager; }

private:
	Renderer renderer;
	NetworkManager networkManager;
	EntityManager entityManager;
	TextureManager textureManager;
	UIManager uiManager;

	bool isOpen;
};

class GameManager
{
	GameManager() {}
	GameManager(GameManager const&) = delete;
	void operator=(GameManager const&) = delete;

	GameInstance gameInstances[GameInstanceCount];
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
};