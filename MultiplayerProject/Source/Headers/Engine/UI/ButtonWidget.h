#include "Engine/UI/ImageWidget.h"

class Button : public BaseWidget
{
public:
	virtual void Initialize(Renderer* renderer) override;
	virtual void Draw(Renderer* renderer, const SDL_Rect& parentRectBounds) override;

	virtual InputResponse TryHandleMousePress(mathfu::Vector<float, 2> mousePressPosition) override;

private:
	Image image;
};