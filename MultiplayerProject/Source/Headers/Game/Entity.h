#pragma once
#include "Engine/BaseObject.h"
#include "Engine/CharacterController.h"
#include "Engine/Rendering/SpriteSheet.h"
#include "Engine/Managers/TextureManager.h"

struct SDL_Texture;
struct SDL_Rect;

class Entity : public BaseObject
{
public:
	friend class EntityManager;

	virtual ~Entity() {};
	virtual void Update(float deltaTime);
	virtual void Render();
	virtual void Destroy();

	void SetController(std::shared_ptr<CharacterController> newController) { controller = newController; }
protected:
	virtual bool Initialize() override;
	void Spawn(mathfu::Vector<float, 2> inPosition);

	void InitTexture(const char* inTextureFilename, mathfu::Vector<float, 2> textureDimensions);
	void ReceiveControllerInput(float deltaTime);

	float width, height;

	mathfu::Vector<float, 2> position;
	SpriteSheet spriteSheet;
	std::shared_ptr<Texture> texture;
	std::shared_ptr<CharacterController> controller;
};