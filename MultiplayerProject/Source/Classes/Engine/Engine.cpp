#include "Engine/Engine.h"

void Engine::Initialize()
{
	uiManager.Initialize();
	renderer.Initialize(640, 480, true);
	inputManager.Initialize();
}

bool Engine::Update(float deltaTime)
{
	uiManager.Update(deltaTime);
	inputManager.Update();
	renderer.Update(deltaTime);

	return !inputManager.WantsToQuit();
}
