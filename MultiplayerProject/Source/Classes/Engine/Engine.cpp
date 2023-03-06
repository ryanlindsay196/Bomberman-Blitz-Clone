#include "Engine/Engine.h"
#include "Game/GameManager.h"

void Engine::Initialize()
{
	GameManager::Get().Initialize();

	renderer.Initialize(480, 640, false, "Blast Engine");
	uiManager.Initialize(&renderer);
}

bool Engine::Update(float deltaTime)
{
	GameManager::Get().Update(deltaTime);

	renderer.Update(deltaTime);
	uiManager.Update();
	uiManager.Draw(&renderer);

	renderer.Render();

	return renderer.IsOpen();
}
