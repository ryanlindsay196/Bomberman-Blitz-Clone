#pragma once
#include "Entity.h"
#include "Engine/Reflection/Reflection.h"

class Player : public Entity
{
public:
	CreateClassMetadata(Player)

	friend class EntityManager;

	virtual void Update(float deltaTime) override;

protected:
	virtual bool Initialize() override;
	virtual void Destroy();

	int health;
};