#include "Game/GameManager.h"
#include <iostream>
#include "SDL.h"

#include "Game/Entity.h"
#include "Game/EntityManager.h"
#include "Game/Player.h"

GameManager& GameManager::Get()
{
	static GameManager instance;
	return instance;
}

bool GameManager::Initialize()
{
	networkManager.Initialize();
	renderer.Initialize();
	inputManager.Initialize();

	//This is here to instantiate an entityManager. This way we can call RPCs on it.
	EntityManager::GetInstance();

	return true;
}

bool GameManager::Update()
{
	static float deltaTime = 0;
	static float frameStart = 0;

	networkManager.Update(deltaTime);
	inputManager.Update();	
	renderer.Update(deltaTime);
	
	frameStart = SDL_GetTicks64();
	
	static EntityManager* entityManager = EntityManager::GetInstance();
	entityManager->UpdateEntities(deltaTime);
	
	networkManager.SerializeNetworkedObjects();
	
	entityManager->RenderEntities();
	
	renderer.Render();
	
	deltaTime = SDL_GetTicks64() - frameStart;

	return !inputManager.WantsToQuit();
}

void GameManager::Destroy()
{
	networkManager.Destroy();
	renderer.Destroy();

	SDL_Quit();
}
