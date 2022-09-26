#pragma once
#include "Entity.h"
#include "Engine/Reflection/Reflection.h"

class Player : public Entity
{
public:
	CreateClassMetadata(Player)

	friend class EntityManager;
public:
	virtual void Update(float deltaTime) override;
private:
	virtual bool Initialize(const char* imagePath, float inWidth, float inHeight, float inPosX, float inPosY) override;
	virtual void Destroy();

	int health;
};