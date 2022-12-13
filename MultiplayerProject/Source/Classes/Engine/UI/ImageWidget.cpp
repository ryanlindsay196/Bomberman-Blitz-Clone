#include "SDL.h"
#include "Engine/UI/ImageWidget.h"
#include "Engine/Rendering/Renderer.h"
#include "Game/GameManager.h"

void Image::Initialize(Renderer* renderer)
{
	texture = GameManager::GetTextureManager()->LoadTexture(renderer, "Resources/Images/HelloWorld.png");
	BaseWidget::Initialize(renderer);
}

void Image::Draw(Renderer * renderer, const SDL_Rect& parentRectBounds)
{
	//Position within the parent container.
	const mathfu::Vector<float, 2> anchorOffset{
		parentRectBounds.w * anchor.GetNormalizedValue().x,
		parentRectBounds.h * anchor.GetNormalizedValue().y 
	};

	float widthInGlobalSpace = GetWidthInGlobalSpace(renderer, parentRectBounds);
	float heightInGlobalSpace = GetHeightInGlobalSpace(renderer, parentRectBounds);

	//Determines the center of this widget
	const mathfu::Vector<float, 2> alignmentPositionOffset{
		widthInGlobalSpace * alignment.GetNormalizedValue().x,
		heightInGlobalSpace * alignment.GetNormalizedValue().y
	};

	SDL_Rect srcRect{ 0, 0, 100, 100 };
	SDL_Rect destRect = { 
		parentRectBounds.x + anchorOffset.x - alignmentPositionOffset.x,
		parentRectBounds.y + anchorOffset.y - alignmentPositionOffset.y,
		widthInGlobalSpace, 
		heightInGlobalSpace };

	renderer->UpdateRender(texture.get(), &srcRect, &destRect);

	BaseWidget::Draw(renderer, destRect);
}