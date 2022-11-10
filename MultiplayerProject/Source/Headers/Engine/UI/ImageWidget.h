#pragma once
#include "Engine/UI/BaseWidget.h"
#include "Engine/Managers/TextureManager.h"

class Image : public BaseWidget
{
public:
	virtual void Initialize(Renderer* renderer) override;
	virtual void Draw(Renderer* renderer) override;

	std::shared_ptr<Texture> texture;
	int width, height;
};