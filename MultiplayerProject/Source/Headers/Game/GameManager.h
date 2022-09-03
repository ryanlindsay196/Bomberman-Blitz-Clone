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

	NetworkManager* GetNetworkManager() { return &networkManager; }
	Renderer* GetRenderer() { return &renderer; }
	InputManager* GetInputManager() { return &inputManager; }

private:
	Renderer renderer;
	InputManager inputManager;
	NetworkManager networkManager;
};