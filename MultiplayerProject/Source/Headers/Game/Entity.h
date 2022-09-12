#pragma once
#include "GameManager.h"
#include "Engine/BaseObject.h"

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
	void Destroy();

protected:
	virtual bool Initialize(const char* imagePath, float inWidth, float inHeight, float inPosX, float inPosY);

	float width, height;

	float posX, posY;
	SDL_Texture* texture;
};