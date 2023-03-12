#include "Engine/UI/ButtonWidget.h"
#include "Engine/Rendering/Renderer.h"

void Button::Initialize(Renderer * renderer)
{
	isPressed = false;
	image.Initialize(renderer);
	BaseWidget::Initialize(renderer);
}

void Button::Draw(Renderer * renderer, const SDL_Rect & parentRectBounds, bool isAnyParentDirty)
{
	float colorMultiplier = isPressed ? 0.8f : 1.0f;

	SDL_Color finalButtonDrawColor =
	{
		color.r * colorMultiplier,
		color.g * colorMultiplier,
		color.b * colorMultiplier,
		color.a * colorMultiplier
	};

	float sizeMultiplier = isPressed ? 0.9f : 1.0f;
	SDL_Rect finalButtonBounds =
	{
		cachedBoundsInGlobalSpace.x + (cachedBoundsInGlobalSpace.w * (1 - sizeMultiplier) / 2),
		cachedBoundsInGlobalSpace.y + (cachedBoundsInGlobalSpace.h * (1 - sizeMultiplier) / 2),
		cachedBoundsInGlobalSpace.w * sizeMultiplier,
		cachedBoundsInGlobalSpace.h * sizeMultiplier,
	};

	SDL_Rect srcRect{ 0,0,100,100 };
	BaseWidget::Draw(renderer, parentRectBounds, isAnyParentDirty);
	renderer->UpdateRender(image.GetTexture().get(), srcRect, finalButtonBounds, finalButtonDrawColor);
}

InputResponse Button::TryHandleMousePress(mathfu::Vector<float, 2> mousePressPosition)
{
	return mathfu::InRange(mousePressPosition.x, (float)cachedBoundsInGlobalSpace.x, (float)cachedBoundsInGlobalSpace.x + (float)cachedBoundsInGlobalSpace.w) &&
		mathfu::InRange(mousePressPosition.y, (float)cachedBoundsInGlobalSpace.y, (float)cachedBoundsInGlobalSpace.y + (float)cachedBoundsInGlobalSpace.h) ?
		InputResponse::Handled : InputResponse::UnHandled;
}

void Button::OnMousePressed(mathfu::Vector<float, 2> mousePressPosition)
{
	isPressed = true;
}

void Button::OnMouseReleased(mathfu::Vector<float, 2> mouseReleasePosition)
{
	isPressed = false;
}
