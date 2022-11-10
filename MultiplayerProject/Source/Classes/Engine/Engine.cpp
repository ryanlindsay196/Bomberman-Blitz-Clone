#include "Engine/Engine.h"

void Engine::Initialize()
{
	renderer.Initialize(640, 480, true);
	uiManager.Initialize(&renderer);
	inputManager.Initialize();
}

bool Engine::Update(float deltaTime)
{
	uiManager.Update(&renderer);
	inputManager.Update();
	renderer.Update(deltaTime);

	renderer.Render();

	return !inputManager.WantsToQuit();
}
