#pragma once
#include <vector>
#include <memory>

#include "Engine/Reflection/Reflection.h"
#include "Engine/BaseObject.h"
#include "mathfu/vector.h"

class Entity;

class EntityManager : public BaseObject
{
public:
	CreateClassMetadata(EntityManager)
	EntityManager();

	EntityManager(EntityManager const&) = delete;
	void operator=(EntityManager const&) = delete;

public:
	static EntityManager* GetInstance();
	template<typename T>
	std::shared_ptr<T> CreateEntity(unsigned int networkID, mathfu::Vector<float, 2> startPosition, mathfu::Vector<int, 2> textureDimensions)
	{
		std::shared_ptr <T> newEntity(new T());
		newEntity->Initialize();
		newEntity->InitializeNetworkID(networkID);
		newEntity->Spawn(startPosition);
		newEntity->InitTexture((char*)"Resources/Images/helloworld.png", textureDimensions);

		entities.push_back(newEntity);
		return newEntity;
	}

	void UpdateEntities(float deltaTime);
	void RenderEntities();

private:
	std::vector<std::shared_ptr<Entity>> entities;
	bool Initialize() { return true; }
};