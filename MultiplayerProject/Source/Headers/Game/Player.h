#pragma once
#include "Entity.h"

class Player : public Entity
{
public:

	friend class EntityManager;

	virtual void Update(float deltaTime) override;

protected:
	virtual bool Initialize() override;
	virtual void Destroy();

	int health;
};