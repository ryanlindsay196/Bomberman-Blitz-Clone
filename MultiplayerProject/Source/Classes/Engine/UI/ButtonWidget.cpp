#include "Engine/UI/ButtonWidget.h"
#include "Engine/Rendering/Renderer.h"

void Button::Initialize(Renderer * renderer)
{
	image.Initialize(renderer);
	BaseWidget::Initialize(renderer);
}

void Button::Draw(Renderer * renderer, const SDL_Rect & parentRectBounds, bool isAnyParentDirty)
{
	SDL_Rect srcRect{ 0,0,100,100 };
	BaseWidget::Draw(renderer, parentRectBounds, isAnyParentDirty);
	renderer->UpdateRender(image.GetTexture().get(), srcRect, cachedBoundsInGlobalSpace);
}

InputResponse Button::TryHandleMousePress(mathfu::Vector<float, 2> mousePressPosition)
{
	return mathfu::InRange(mousePressPosition.x, (float)cachedBoundsInGlobalSpace.x, (float)cachedBoundsInGlobalSpace.x + (float)cachedBoundsInGlobalSpace.w) &&
		mathfu::InRange(mousePressPosition.y, (float)cachedBoundsInGlobalSpace.y, (float)cachedBoundsInGlobalSpace.y + (float)cachedBoundsInGlobalSpace.h) ?
		InputResponse::Handled : InputResponse::UnHandled;
}
