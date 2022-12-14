#include "Engine/UI/ImageWidget.h"

class Button : public BaseWidget
{
public:
	virtual void Initialize(Renderer* renderer) override;
	virtual void Draw(Renderer* renderer, const SDL_Rect& parentRectBounds) override;

private:
	Image image;
};