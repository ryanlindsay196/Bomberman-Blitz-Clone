#include "SDL.h"
#include "SDL_image.h"

#include "Game/Entity.h"
#include "Engine/Rendering/Renderer.h"

bool Entity::Initialize(const char *imagePath, float inWidth, float inHeight, mathfu::Vector<float, 2> inPosition)
{
	width = inWidth;
	height = inHeight;
	position.x = inPosition.x;
	position.y = inPosition.y;

	GameManager& gameManager = GameManager::Get();
	texture = gameManager.GetRenderer()->LoadTexture(imagePath);

	RegisterSelfAsNetworked();

	return texture != nullptr;
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
