#pragma once
#include "Engine/UI/BaseWidget.h"
#include "Engine/Managers/TextureManager.h"

class Image : public BaseWidget
{
public:
	virtual void Initialize(Renderer* renderer) override;
	virtual void Draw(Renderer* renderer, const SDL_Rect& parentRectBounds) override;

	std::shared_ptr<Texture> GetTexture() { return texture; }
private:
	std::shared_ptr<Texture> texture;
};