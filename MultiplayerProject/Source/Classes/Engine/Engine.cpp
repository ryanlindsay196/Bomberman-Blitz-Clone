#include "Engine/Engine.h"

void Engine::Initialize()
{
	renderer.Initialize(640, 480, true);
	inputManager.Initialize();
}

bool Engine::Update(float deltaTime)
{
	inputManager.Update();
	renderer.Update(deltaTime);

	return !inputManager.WantsToQuit();
}
