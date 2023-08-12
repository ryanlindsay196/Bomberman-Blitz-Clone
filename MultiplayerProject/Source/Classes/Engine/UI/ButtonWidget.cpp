#include "Engine/UI/ButtonWidget.h"
#include "Engine/Rendering/Renderer.h"

#include "Engine/Managers/AssetManager.h"

RegisterStaticAsset(Button);

void Button::Initialize(Renderer * renderer, rapidjson::GenericArray<false, rapidjson::Value::ValueType>::PlainType* widgetData)
{
	isPressed = false;

	image.Initialize(renderer, widgetData);
	BaseWidget::Initialize(renderer, widgetData);
}

void Button::Draw(Renderer * renderer, const SDL_Rect & parentRectBounds, bool isAnyParentDirty)
{
	float colorMultiplier = isPressed ? 0.8f : 1.0f;

	SDL_Color finalButtonDrawColor =
	{
		(Uint8)(color.r * colorMultiplier),
		(Uint8)(color.g * colorMultiplier),
		(Uint8)(color.b * colorMultiplier),
		(Uint8)(color.a * colorMultiplier)
	};

	float sizeMultiplier = isPressed ? 0.9f : 1.0f;
	SDL_Rect finalButtonBounds =
	{
		(int)(cachedBoundsInGlobalSpace.x + (cachedBoundsInGlobalSpace.w * (1 - sizeMultiplier) / 2)),
		(int)(cachedBoundsInGlobalSpace.y + (cachedBoundsInGlobalSpace.h * (1 - sizeMultiplier) / 2)),
		(int)(cachedBoundsInGlobalSpace.w * sizeMultiplier),
		(int)(cachedBoundsInGlobalSpace.h * sizeMultiplier),
	};

	SDL_Rect srcRect{ 0,0,100,100 };
	BaseWidget::Draw(renderer, parentRectBounds, isAnyParentDirty);
	renderer->UpdateRender(image.GetTexture().get(), srcRect, finalButtonBounds, finalButtonDrawColor);
}

InputResponse Button::TryHandleMousePress(mathfu::Vector<int, 2> mousePressPosition)
{
	return mathfu::InRange(mousePressPosition.x, cachedBoundsInGlobalSpace.x, cachedBoundsInGlobalSpace.x + cachedBoundsInGlobalSpace.w) &&
		mathfu::InRange(mousePressPosition.y, cachedBoundsInGlobalSpace.y, cachedBoundsInGlobalSpace.y + cachedBoundsInGlobalSpace.h) ?
		InputResponse::Handled : InputResponse::UnHandled;
}

void Button::OnMousePressed(mathfu::Vector<int, 2> mousePressPosition)
{
	isPressed = true;
}

void Button::OnMouseReleased(mathfu::Vector<int, 2> mouseReleasePosition)
{
	isPressed = false;
}
