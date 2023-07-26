#include "Engine/UI/ImageWidget.h"

class Button : public BaseWidget
{
public:
	virtual void Initialize(Renderer* renderer, rapidjson::GenericArray<false, rapidjson::Value::ValueType>::PlainType* widgetData) override;
	virtual void Draw(Renderer* renderer, const SDL_Rect& parentRectBounds, bool isAnyParentDirty) override;

	virtual InputResponse TryHandleMousePress(mathfu::Vector<float, 2> mousePressPosition) override;

	virtual void OnMousePressed(mathfu::Vector<float, 2> mousePressPosition) override;
	virtual void OnMouseReleased(mathfu::Vector<float, 2> mouseReleasePosition) override;

private:
	Image image;
	bool isPressed;
};