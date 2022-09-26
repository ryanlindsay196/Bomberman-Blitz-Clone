#include "SDL.h"
#include "SDL_image.h"

#include "Game/Entity.h"
#include "Engine/Rendering/Renderer.h"

bool Entity::Initialize(const char *imagePath, float inWidth, float inHeight, float inPosX, float inPosY)
{
	width = inWidth;
	height = inHeight;
	posX = inPosX;
	posY = inPosY;

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
		posX += controller->GetLeftAxis().x * deltaTime;
		posY += controller->GetLeftAxis().y * deltaTime;
	}
}

void Entity::Update(float deltaTime)
{
	ReceiveControllerInput(deltaTime);
}

void Entity::Render()
{
	SDL_Rect destRect;
	destRect.x = posX;
	destRect.y = posY;
	destRect.w = width;
	destRect.h = height;

	GameManager& gameManager = GameManager::Get();
	gameManager.GetRenderer()->UpdateRender(texture, nullptr, &destRect);
}

void Entity::Destroy()
{
	SDL_DestroyTexture(texture);
}
