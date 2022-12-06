#include "SDL.h"
#include "Engine/UI/ImageWidget.h"
#include "Engine/Rendering/Renderer.h"
#include "Game/GameManager.h"

void Image::Initialize(Renderer* renderer)
{
	texture = GameManager::GetTextureManager()->LoadTexture(renderer, "Resources/Images/HelloWorld.png");
	BaseWidget::Initialize(renderer);
}

void Image::Draw(Renderer * renderer)
{
	const mathfu::Vector<float, 2> anchorPosition{
		renderer->GetViewportWidth() * anchor.GetNormalizedValue().x,
		renderer->GetViewportHeight() * anchor.GetNormalizedValue().y };

	const mathfu::Vector<float, 2> alignmentPositionOffset{
		width * alignment.GetNormalizedValue().x,
		height * alignment.GetNormalizedValue().y };

	const mathfu::Vector<float, 2> offsetPosition{ renderer->GetViewportWidth() * offset.x, renderer->GetViewportHeight() * offset.y };

	SDL_Rect srcRect{ 0, 0, 100, 100 };
	SDL_Rect destRect = { 
		anchorPosition.x - alignmentPositionOffset.x + offsetPosition.x, 
		anchorPosition.y - alignmentPositionOffset.y + offsetPosition.y, 
		width, 
		height };

	renderer->UpdateRender(texture.get(), &srcRect, &destRect);
	BaseWidget::Draw(renderer);
}