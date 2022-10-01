#pragma once
#include "GameManager.h"
#include "Engine/BaseObject.h"
#include "Engine/CharacterController.h"

class SDL_Surface;
class SDL_Texture;

class Entity : public BaseObject
{
public:
	CreateClassMetadata(Entity)
	friend class EntityManager;


	virtual ~Entity() {};
	virtual void Update(float deltaTime);
	virtual void Render();
	virtual void Destroy();

protected:
	virtual bool Initialize(const char* imagePath, float inWidth, float inHeight, mathfu::Vector<float, 2> inPosition) override;
	void ReceiveControllerInput(float deltaTime);

	float width, height;

	//TODO: Change posX and posY to a vector2
	mathfu::Vector<float, 2> position;
	SDL_Texture* texture;
	CharacterController* controller;
};