#include "SDL.h"
#include "Engine/UI/ImageWidget.h"
#include "Engine/Rendering/Renderer.h"
#include "Game/GameManager.h"

void Image::Initialize(Renderer* renderer)
{
	texture = GameManager::GetTextureManager()->LoadTexture(renderer, "Resources/Images/HelloWorld.png");
	BaseWidget::Initialize(renderer);
}

void Image::Draw(Renderer * renderer, const SDL_Rect& parentRectBounds)
{
	SDL_Rect srcRect { 0, 0, 100, 100 };
	SDL_Rect destRect = CalculateBoundsInGlobalSpace(renderer, parentRectBounds);
	renderer->UpdateRender(texture.get(), &srcRect, &destRect);

	BaseWidget::Draw(renderer, destRect);
}