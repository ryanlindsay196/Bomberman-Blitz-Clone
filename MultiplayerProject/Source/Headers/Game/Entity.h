#pragma once
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
	virtual bool Initialize() override;
	void Spawn(mathfu::Vector<float, 2> inPosition);

	void InitTexture(const char* inTextureFilename, mathfu::Vector<int, 2> textureDimensions);
	void ReceiveControllerInput(float deltaTime);

	float width, height;

	mathfu::Vector<float, 2> position;
	SDL_Texture* texture;
	CharacterController* controller;
};