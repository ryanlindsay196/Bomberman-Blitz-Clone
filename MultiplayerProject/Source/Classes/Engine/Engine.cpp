#include "Engine/Engine.h"
#include "Game/GameManager.h"

void Engine::Initialize()
{
	GameManager::Get().Initialize();

	renderer.Initialize(480, 640, true, "Blast Engine");
	uiManager.Initialize(&renderer);
}

bool Engine::Update(float deltaTime)
{
	GameManager::Get().Update(deltaTime);

	uiManager.Update(&renderer);
	renderer.Update(deltaTime);

	renderer.Render();

	return renderer.IsOpen();
}
