#pragma once
#include <vector>
#include <memory>

#include "mathfu/vector.h"

class Entity;

class EntityManager
{
	EntityManager() {};

	EntityManager(EntityManager const&) = delete;
	void operator=(EntityManager const&) = delete;

public:
	static EntityManager* GetInstance();
	template<typename T>
	T* CreateEntity(const char* fileName, mathfu::Vector<int, 2> textureDimensions)
	{
		T* newEntity = new T();
		newEntity->Initialize();
		newEntity->Spawn(mathfu::Vector<float, 2> {0, 0});
		newEntity->InitTexture(fileName, textureDimensions);

		entities.push_back(newEntity);
		return (T*)entities[entities.size() - 1];
	}

	void UpdateEntities(float deltaTime);
	void RenderEntities();

private:
	std::vector<Entity*> entities;
};