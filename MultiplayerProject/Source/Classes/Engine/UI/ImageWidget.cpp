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
	JsonDataPopulator::PopulateVarData(mv_path, widgetData, path);

	CreateVariableMetadata(Image, srcRect);
	JsonDataPopulator::PopulateVarData(mv_srcRect, widgetData, srcRect.x, srcRect.y, srcRect.w, srcRect.h);
	
	texture = renderer->GetTextureManager()->LoadTexture(renderer, path);
}

void Image::Draw(Renderer * renderer, const SDL_Rect& parentRectBounds, bool isAnyParentDirty)
{
	BaseWidget::Draw(renderer, parentRectBounds, isAnyParentDirty);

	renderer->UpdateRender(texture.get(), srcRect, cachedBoundsInGlobalSpace, color);
}