#pragma once
#include "Engine/UI/BaseWidget.h"
#include "Engine/Managers/TextureManager.h"

class Image : public BaseWidget
{
public:
	virtual void Initialize(Renderer* renderer, rapidjson::GenericArray<false, rapidjson::Value::ValueType>::PlainType* widgetData) override;
	virtual void Draw(Renderer* renderer, const SDL_Rect& parentRectBounds, bool isAnyParentDirty) override;
	
	virtual InputResponse TryHandleMousePress(mathfu::Vector<int, 2> mousePressPosition) override { return InputResponse::UnHandled; }

	std::shared_ptr<Texture> GetTexture() { return texture; }
	SDL_Rect GetSrcRect() { return srcRect; }
private:
	std::shared_ptr<Texture> texture;

	char* path;
	SDL_Rect srcRect;
};