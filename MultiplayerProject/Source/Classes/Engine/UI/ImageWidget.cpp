#include "SDL.h"
#include "Engine/UI/ImageWidget.h"
#include "Engine/Rendering/Renderer.h"
#include "Engine/Managers/AssetManager.h"
#include "Game/GameManager.h"

CreateClassMetadata(Image);
RegisterStaticAsset(Image);

void Image::Initialize(Renderer* renderer, rapidjson::GenericArray<false, rapidjson::Value::ValueType>::PlainType* widgetData)
{
	BaseWidget::Initialize(renderer, widgetData);

	CreateVariableMetadata(Image, path);
	PopulateWidgetData(mv_path, widgetData, path);
	
	texture = renderer->GetTextureManager()->LoadTexture(renderer, path);
}

void Image::Draw(Renderer * renderer, const SDL_Rect& parentRectBounds, bool isAnyParentDirty)
{
	SDL_Rect srcRect { 0, 0, 100, 100 };
	BaseWidget::Draw(renderer, parentRectBounds, isAnyParentDirty);

	renderer->UpdateRender(texture.get(), srcRect, cachedBoundsInGlobalSpace, color);
}