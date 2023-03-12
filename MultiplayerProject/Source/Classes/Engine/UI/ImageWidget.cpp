#include "SDL.h"
#include "Engine/UI/ImageWidget.h"
#include "Engine/Rendering/Renderer.h"
#include "Game/GameManager.h"

void Image::Initialize(Renderer* renderer)
{
	texture = renderer->GetTextureManager()->LoadTexture(renderer, "Resources/Images/HelloWorld.png");
	BaseWidget::Initialize(renderer);
}

void Image::Draw(Renderer * renderer, const SDL_Rect& parentRectBounds, bool isAnyParentDirty)
{
	SDL_Rect srcRect { 0, 0, 100, 100 };
	BaseWidget::Draw(renderer, cachedBoundsInGlobalSpace, isAnyParentDirty);

	renderer->UpdateRender(texture.get(), srcRect, cachedBoundsInGlobalSpace, color);
}