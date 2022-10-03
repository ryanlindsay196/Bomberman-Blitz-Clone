#include "SDL.h"
#include "SDL_image.h"

#include "Game/Entity.h"
#include "Game/GameManager.h"

#include "Engine/Rendering/Renderer.h"

bool Entity::Initialize()
{
	RegisterSelfAsNetworked();

	return true;
}

void Entity::Spawn(mathfu::Vector<float, 2> inPosition)
{
	//TODO: Set up logic so we can't spawn if we're already spawned
	position.x = inPosition.x;
	position.y = inPosition.y;
}

void Entity::InitTexture(const char* inTextureFilename, mathfu::Vector<int, 2> textureDimensions)
{
	width = textureDimensions.x;
	height = textureDimensions.y;

	GameManager& gameManager = GameManager::Get();
	texture = gameManager.GetRenderer()->LoadTexture(inTextureFilename);
}

void Entity::ReceiveControllerInput(float deltaTime)
{
	if (controller)
	{
		controller->UpdateInputs();
		position.x += controller->GetLeftAxis().x * deltaTime;
		position.y += controller->GetLeftAxis().y * deltaTime;
	}
}

void Entity::Update(float deltaTime)
{
	ReceiveControllerInput(deltaTime);
}

void Entity::Render()
{
	SDL_Rect destRect;
	destRect.x = position.x;
	destRect.y = position.y;
	destRect.w = width;
	destRect.h = height;

	GameManager& gameManager = GameManager::Get();
	gameManager.GetRenderer()->UpdateRender(texture, nullptr, &destRect);
}

void Entity::Destroy()
{
	SDL_DestroyTexture(texture);
}
