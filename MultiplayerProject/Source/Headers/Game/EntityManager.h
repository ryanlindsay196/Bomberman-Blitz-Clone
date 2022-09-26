#pragma once
#include <vector>
#include <memory>

class Entity;

class EntityManager
{
	EntityManager() {};

	EntityManager(EntityManager const&) = delete;
	void operator=(EntityManager const&) = delete;

public:
	static EntityManager* GetInstance();
	template<typename T>
	T& CreateEntity()
	{
		T newEntity = T();
		newEntity.Initialize("Resources/Images/helloworld.png", 300, 200, 0, 0);

		entities.push_back(newEntity);
		return (T&)entities[entities.size() - 1];
	}

	void UpdateEntities(float deltaTime);
	void RenderEntities();

private:
	std::vector<Entity> entities;
};