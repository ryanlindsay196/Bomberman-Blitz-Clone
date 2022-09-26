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
	virtual bool Initialize(const char* imagePath, float inWidth, float inHeight, float inPosX, float inPosY);
	void ReceiveControllerInput(float deltaTime);

	float width, height;

	//TODO: Change posX and posY to a vector2
	float posX, posY;
	SDL_Texture* texture;
	CharacterController* controller;
};