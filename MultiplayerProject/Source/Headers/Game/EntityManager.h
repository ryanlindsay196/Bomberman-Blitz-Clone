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
	std::shared_ptr<T> CreateEntity(const char* fileName, mathfu::Vector<int, 2> textureDimensions)
	{
		std::shared_ptr <T> newEntity(new T());
		newEntity->Initialize();
		newEntity->Spawn(mathfu::Vector<float, 2> {0, 0});
		newEntity->InitTexture(fileName, textureDimensions);

		entities.push_back(newEntity);
		return newEntity;
	}

	void UpdateEntities(float deltaTime);
	void RenderEntities();

private:
	std::vector<std::shared_ptr<Entity>> entities;
};