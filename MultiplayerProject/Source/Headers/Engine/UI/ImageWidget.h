#pragma once
#include "Engine/UI/BaseWidget.h"
#include "Engine/Managers/TextureManager.h"

class Image : public BaseWidget
{
public:
	virtual void Initialize(Renderer* renderer) override;
	virtual void Draw(Renderer* renderer) override;

	virtual void SetWidth(int inWidth) { width = inWidth; }
	virtual void SetHeight(int inHeight) { height = inHeight; }

	std::shared_ptr<Texture> texture;
	int width, height;
};