#include "Game/EntityManager.h"
#include "Game/Entity.h"

EntityManager * EntityManager::GetInstance()
{
	static EntityManager* instance = new EntityManager();
	return instance;
}

void EntityManager::UpdateEntities(float deltaTime)
{
	for (Entity* entity : entities)
	{
		entity->Update(deltaTime);
	}
}

void EntityManager::RenderEntities()
{
	for (Entity* entity : entities)
	{
		entity->Render();
	}
}
