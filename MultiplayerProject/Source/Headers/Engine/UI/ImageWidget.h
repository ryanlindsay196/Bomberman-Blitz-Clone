#pragma once
#include "Engine/UI/BaseWidget.h"
#include "Engine/Managers/TextureManager.h"

class Image : public BaseWidget
{
public:
	CreateClassMetadata(Image)
	virtual void Initialize(Renderer* renderer, rapidjson::GenericArray<false, rapidjson::Value::ValueType>::PlainType* widgetData) override;
	virtual void Draw(Renderer* renderer, const SDL_Rect& parentRectBounds, bool isAnyParentDirty) override;

	std::shared_ptr<Texture> GetTexture() { return texture; }
private:
	std::shared_ptr<Texture> texture;

	char* path;
};