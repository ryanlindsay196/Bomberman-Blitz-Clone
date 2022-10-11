#pragma once
#include "Engine/Rendering/Renderer.h"
#include "Engine/InputManager.h"
#include "Engine/Networking/NetworkManager.h"

class GameManager
{
	GameManager() {}
	GameManager(GameManager const&) = delete;
	void operator=(GameManager const&) = delete;

public:
	static GameManager& Get();

	bool Initialize();
	bool Update();
	void Destroy();

	static NetworkManager& GetNetworkManager() { return Get().networkManager; }
	static Renderer* GetRenderer() { return &Get().renderer; }
	static InputManager* GetInputManager() { return &Get().inputManager; }

private:
	Renderer renderer;
	InputManager inputManager;
	NetworkManager networkManager;
};