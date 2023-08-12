#include "Engine/Rendering/Renderer.h"
#include "Engine/UI/BaseWidget.h"

void BaseWidget::Initialize(Renderer* renderer, rapidjson::GenericArray<false, rapidjson::Value::ValueType>::PlainType* widgetData)
{
	strcpy_s(name, sizeof(name) + 1, "12345678901234567890");
	isTransformDirty = true;
	color = SDL_Color{ 255,255,255,255 };

	if (!widgetData)
		return;

	CreateVariableMetadata(BaseWidget, sizeInLocalSpace);
	PopulateWidgetData(mv_sizeInLocalSpace, widgetData, sizeInLocalSpace.x, sizeInLocalSpace.y);

	CreateVariableMetadata(BaseWidget, anchor);
	PopulateWidgetData(mv_anchor, widgetData, anchor.normalizedValue.x, anchor.normalizedValue.y);

	CreateVariableMetadata(BaseWidget, alignment);
	PopulateWidgetData(mv_alignment, widgetData, alignment.normalizedValue.x, alignment.normalizedValue.y);
}

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

	mathfu::Vector<int, 2> mousePosition(mouseInput.e.button.x, mouseInput.e.button.y);

	bool isClickHandled = TryHandleMousePress(mousePosition) == InputResponse::Handled;
	if (isClickHandled)
	{
		MousePressed(mousePosition);
		return this;
	}
	return nullptr;
}

void BaseWidget::OnMousePressed(mathfu::Vector<int, 2> mousePressPosition)
{
}

void BaseWidget::OnMouseReleased(mathfu::Vector<int, 2> mouseReleasePosition)
{
}

void BaseWidget::MousePressed(mathfu::Vector<int, 2> mousePressPosition)
{
	OnMousePressed(mousePressPosition);
	onMousePressedDel.Execute();

}

void BaseWidget::MouseReleased(mathfu::Vector<int, 2> mouseReleasePosition)
{
	OnMouseReleased(mouseReleasePosition);
	onMouseReleasedDel.Execute();
}

InputResponse BaseWidget::TryHandleMousePress(mathfu::Vector<int, 2> mousePressPosition)
{
	//TODO: Switch to InputResponse::UnHandled after creating a main menu widget for the engine.
	return InputResponse::Handled;
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

	float widthInGlobalSpace = parentRectBounds.w * sizeInLocalSpace.x / (float)renderer->GetViewportWidth();

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

	float heightInGlobalSpace = parentRectBounds.h * sizeInLocalSpace.y / (float)renderer->GetViewportHeight();

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
		parentRectBounds.w * anchor.normalizedValue.x,
		parentRectBounds.h * anchor.normalizedValue.y
	};

	float widthInGlobalSpace = GetWidthInGlobalSpace(renderer, parentRectBounds);
	float heightInGlobalSpace = GetHeightInGlobalSpace(renderer, parentRectBounds);

	//Determines the center of this widget
	const mathfu::Vector<float, 2> alignmentPositionOffset{
		widthInGlobalSpace * alignment.normalizedValue.x,
		heightInGlobalSpace * alignment.normalizedValue.y
	};

	SDL_Rect destRect = {
		parentRectBounds.x + (int)(anchorOffset.x - alignmentPositionOffset.x),
		parentRectBounds.y + (int)(anchorOffset.y - alignmentPositionOffset.y),
		(int)widthInGlobalSpace,
		(int)heightInGlobalSpace };

	return destRect;
}

void BaseWidget::CopyVariableFromJSON(Variable & destinationVar, const rapidjson::GenericArray<false, rapidjson::Value::ValueType>::PlainType & sourceJSON)
{
	if (sourceJSON.IsInt())
	{
		int intToCopy = sourceJSON.GetInt();
		memcpy(destinationVar.v, &intToCopy, destinationVar.m->SizeOf());
	}
	else if (sourceJSON.IsFloat())
	{
		float floatToCopy = sourceJSON.GetFloat();
		memcpy(destinationVar.v, &floatToCopy, destinationVar.m->SizeOf());
	}
	else if (sourceJSON.IsString())
	{
		*(char**)destinationVar.v = const_cast<char*>(sourceJSON.GetString());
	}
}