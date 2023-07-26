#include "SDL.h"
#include "Engine/UI/ImageWidget.h"
#include "Engine/Rendering/Renderer.h"
#include "Engine/Managers/AssetManager.h"
#include "Game/GameManager.h"

RegisterStaticAsset(Image);

void Image::Initialize(Renderer* renderer, rapidjson::GenericArray<false, rapidjson::Value::ValueType>::PlainType* widgetData)
{
	BaseWidget::Initialize(renderer, widgetData);

	assert(widgetData);

	CreateVariableMetadata(Image, path);
	rapidjson::Value::MemberIterator pathMemberIterator = widgetData->FindMember(mv_path.GetName());
	path = const_cast<char*>(pathMemberIterator->value.GetString());
	texture = renderer->GetTextureManager()->LoadTexture(renderer, path);
}

void Image::Draw(Renderer * renderer, const SDL_Rect& parentRectBounds, bool isAnyParentDirty)
{
	SDL_Rect srcRect { 0, 0, 100, 100 };
	BaseWidget::Draw(renderer, parentRectBounds, isAnyParentDirty);

	renderer->UpdateRender(texture.get(), srcRect, cachedBoundsInGlobalSpace, color);
}