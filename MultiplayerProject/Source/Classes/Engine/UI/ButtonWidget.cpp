#include "Engine/UI/ButtonWidget.h"
#include "Engine/Rendering/Renderer.h"

void Button::Initialize(Renderer * renderer)
{
	image.Initialize(renderer);
}

void Button::Draw(Renderer * renderer, const SDL_Rect & parentRectBounds)
{
	SDL_Rect srcRect{ 0,0,100,100 };
	SDL_Rect destRect = CalculateBoundsInGlobalSpace(renderer, parentRectBounds);

	renderer->UpdateRender(image.GetTexture().get(), &srcRect, &destRect);
}
