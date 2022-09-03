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

	EntityManager* entityManager = EntityManager::GetInstance();
	entityManager->CreateEntity<Player>();

	return true;
}

bool GameManager::Update()
{
	networkManager.HandlePackets();
	inputManager.Update();
	
	static float deltaTime = 0;
	static float frameStart = 0;
	
	frameStart = SDL_GetTicks64();
	
	static EntityManager* entityManager = EntityManager::GetInstance();
	entityManager->UpdateEntities(deltaTime);
	
	networkManager.SendSerializedData();
	
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
