#include "Engine/Rendering/Renderer.h"
#include "Engine/UI/BaseWidget.h"

void BaseWidget::AddChild(BaseWidget* newChild)
{
	children.push_back(newChild);
}

void BaseWidget::RemoveChild(BaseWidget * childToRemove)
{
	for (auto it = children.begin(); it != children.end(); ++it)
	{
		if (childToRemove == *it._Ptr)
		{
			children.erase(it);
			return;
		}
	}
}

void BaseWidget::RemoveFromParent()
{
	if (GetParent())
	{
		parent->RemoveChild(this);
	}
}

void BaseWidget::SetParent(BaseWidget * inParent)
{
	parent = inParent;
}

bool BaseWidget::ProcessMouseInput(mathfu::Vector<float, 2> mousePosition)
{
	for (BaseWidget* currentChild : children)
	{
		assert(currentChild);

		if (currentChild->ProcessMouseInput(mousePosition))
		{
			return true;
		}
	}
	bool isClickHandled = TryHandleMousePress(mousePosition) == InputResponse::Handled;
	if (isClickHandled)
	{
		OnMousePressed(mousePosition);
	}
	return isClickHandled;
}

void BaseWidget::OnMousePressed(mathfu::Vector<float, 2> mousePressPosition)
{
}

InputResponse BaseWidget::TryHandleMousePress(mathfu::Vector<float, 2> mousePressPosition)
{
	return InputResponse::UnHandled;
}

void BaseWidget::Draw(Renderer* renderer, const SDL_Rect& parentRectBounds)
{
	for (BaseWidget* child : children)
	{
		if (child)
		{
			child->Draw(renderer, parentRectBounds);
		}
	}
}

float BaseWidget::GetWidthInGlobalSpace(const Renderer* const renderer, const SDL_Rect& parentRectBounds) const
{
	assert(renderer);

	float parentAspectRatio = (float)parentRectBounds.w / parentRectBounds.h;

	float viewportAspectRatioAdjustment = renderer->GetAspectRatio() / parentAspectRatio;

	float widthInGlobalSpace = parentRectBounds.w * widthInLocalSpace / (float)renderer->GetViewportWidth();

	if (viewportAspectRatioAdjustment < 1)
	{
		widthInGlobalSpace *= viewportAspectRatioAdjustment;
	}

	return widthInGlobalSpace;
}

float BaseWidget::GetHeightInGlobalSpace(const Renderer* const renderer, const SDL_Rect& parentRectBounds) const
{
	assert(renderer);

	float parentAspectRatio = (float)parentRectBounds.w / parentRectBounds.h;

	float viewportAspectRatioAdjustment = renderer->GetAspectRatio() / parentAspectRatio;

	float heightInGlobalSpace = parentRectBounds.h * heightInLocalSpace / (float)renderer->GetViewportHeight();

	if (viewportAspectRatioAdjustment > 1)
	{
		heightInGlobalSpace /= viewportAspectRatioAdjustment;
	}

	return heightInGlobalSpace;
}

SDL_Rect BaseWidget::CalculateBoundsInGlobalSpace(const Renderer* const renderer, const SDL_Rect& parentRectBounds) const
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

	return destRect;
}

