#include <SDL.h>

#ifdef RunInEngine
#include "Engine/Engine.h"
Engine engine;
#else
#include "Game/GameManager.h"
#endif

Engine* GetEngine()
{
#ifdef RunInEngine
	return &engine;
#else
	return nullptr;
#endif
}

int main(int argc, char* args[])
{
	bool isInGameLoop = true;
	
	float deltaTime = 0;
	float frameStart = 0;
#ifdef RunInEngine
	engine.Initialize();

	while (isInGameLoop)
	{
		frameStart = SDL_GetTicks64();
		isInGameLoop = engine.Update(deltaTime);
		deltaTime = SDL_GetTicks64() - frameStart;
	}
#else
	GameManager& gameManager = GameManager::Get();
	gameManager.Initialize();
	
	while (isInGameLoop)
	{
		frameStart = SDL_GetTicks64();
		isInGameLoop = gameManager.Update(deltaTime);
		deltaTime = SDL_GetTicks64() - frameStart;
	}
	
	gameManager.Destroy();
#endif

	return 0;
}
