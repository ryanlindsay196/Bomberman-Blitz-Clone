#include "Game/GameManager.h"

GameManager& GameManager::Get()
{
	static GameManager instance;
	return instance;
}

bool GameManager::Initialize()
{
	networkManager.Initialize();
	renderer.Initialize(640, 480, false);
	uiManager.Initialize(&renderer);
	inputManager.Initialize();
	entityManager.Initialize();

	return true;
}

bool GameManager::Update(float deltaTime)
{
	networkManager.Update(deltaTime);
	inputManager.Update();	
	renderer.Update(deltaTime);
	
	entityManager.UpdateEntities(deltaTime);
	
	networkManager.SerializeNetworkedObjects();
	
	entityManager.RenderEntities();

	uiManager.Update(&renderer);
	
	renderer.Render();
	return !inputManager.WantsToQuit();
}

void GameManager::Destroy()
{
	networkManager.Destroy();
	renderer.Destroy();

	SDL_Quit();
}
