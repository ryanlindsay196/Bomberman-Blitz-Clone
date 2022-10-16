#pragma once
#include "Engine/BaseObject.h"
#include "Engine/CharacterController.h"
#include "Engine/Rendering/SpriteSheet.h"

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

	void SetController(std::shared_ptr<CharacterController> newController) { controller = newController; }
protected:
	virtual bool Initialize() override;
	void Spawn(mathfu::Vector<float, 2> inPosition);

	void InitTexture(const char* inTextureFilename, mathfu::Vector<int, 2> textureDimensions);
	void ReceiveControllerInput(float deltaTime);

	float width, height;

	mathfu::Vector<float, 2> position;
	SDL_Texture* texture;
	SpriteSheet spriteSheet;
	std::shared_ptr<CharacterController> controller;
};