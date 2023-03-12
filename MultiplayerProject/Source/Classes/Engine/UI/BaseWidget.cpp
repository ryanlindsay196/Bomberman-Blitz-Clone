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

BaseWidget* BaseWidget::ProcessMouseInput(Input& mouseInput)
{
	for (BaseWidget* currentChild : children)
	{
		assert(currentChild);

		if (currentChild->ProcessMouseInput(mouseInput))
		{
			return currentChild;
		}
	}

	mathfu::Vector<float, 2> mousePosition(mouseInput.e.button.x, mouseInput.e.button.y);

	bool isClickHandled = TryHandleMousePress(mousePosition) == InputResponse::Handled;
	if (isClickHandled)
	{
		MousePressed(mousePosition);
		return this;
	}
	return nullptr;
}

void BaseWidget::OnMousePressed(mathfu::Vector<float, 2> mousePressPosition)
{
}

void BaseWidget::OnMouseReleased(mathfu::Vector<float, 2> mouseReleasePosition)
{
}

void BaseWidget::MousePressed(mathfu::Vector<float, 2> mousePressPosition)
{
	OnMousePressed(mousePressPosition);
	onMousePressedDel.Execute();

}

void BaseWidget::MouseReleased(mathfu::Vector<float, 2> mouseReleasePosition)
{
	OnMouseReleased(mouseReleasePosition);
	onMouseReleasedDel.Execute();
}

InputResponse BaseWidget::TryHandleMousePress(mathfu::Vector<float, 2> mousePressPosition)
{
	return InputResponse::UnHandled;
}

void BaseWidget::Draw(Renderer* renderer, const SDL_Rect& parentRectBounds, bool isAnyParentDirty)
{
	bool selfOrParentHasDirtyTransform = isTransformDirty || isAnyParentDirty;

	if (selfOrParentHasDirtyTransform)
	{
		cachedBoundsInGlobalSpace = CalculateBoundsInGlobalSpace(renderer, parentRectBounds);
	}

	for (BaseWidget* child : children)
	{
		if (child)
		{
			child->Draw(renderer, cachedBoundsInGlobalSpace, selfOrParentHasDirtyTransform);
		}
	}

	isTransformDirty = false;
}

float BaseWidget::GetWidthInGlobalSpace(const Renderer* const renderer, const SDL_Rect& parentRectBounds) const
{
	assert(renderer);

	float parentAspectRatio = (float)parentRectBounds.w / parentRectBounds.h;

	float viewportAspectRatioAdjustment = renderer->GetAspectRatio() / parentAspectRatio;

	float widthInGlobalSpace = parentRectBounds.w * boundsInLocalSpace.w / (float)renderer->GetViewportWidth();

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

	float heightInGlobalSpace = parentRectBounds.h * boundsInLocalSpace.h / (float)renderer->GetViewportHeight();

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

	SDL_Rect destRect = {
		parentRectBounds.x + anchorOffset.x - alignmentPositionOffset.x,
		parentRectBounds.y + anchorOffset.y - alignmentPositionOffset.y,
		widthInGlobalSpace,
		heightInGlobalSpace };

	return destRect;
}

