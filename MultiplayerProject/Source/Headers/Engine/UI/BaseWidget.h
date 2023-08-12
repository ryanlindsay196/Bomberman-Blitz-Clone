#pragma once
#include <vector>
#include "mathfu/vector.h"
#include "SDL_rect.h"
#include "Engine/Delegate.h"

#include "Engine/InputManager.h"
#include "Engine/Reflection/Reflection.h"

#include "rapidjson/document.h"

class Renderer;

enum class InputResponse
{
	Handled,
	UnHandled
};

struct Anchor
{
public:
	Anchor() : normalizedValue(0, 0) {}
	Anchor(mathfu::Vector<float, 2> inNormalizedValue) : normalizedValue(inNormalizedValue) {}
	
	static const Anchor TopLeft()			{ return Anchor(mathfu::Vector<float, 2>(0.0f, 0.0f)); }
	static const Anchor CenterLeft()		{ return Anchor(mathfu::Vector<float, 2>(0.0f, 0.5f)); }
	static const Anchor BottomLeft()		{ return Anchor(mathfu::Vector<float, 2>(0.0f, 1.0f)); }
	static const Anchor TopCenter()			{ return Anchor(mathfu::Vector<float, 2>(0.5f, 0.0f)); }
	static const Anchor CenterCenter()		{ return Anchor(mathfu::Vector<float, 2>(0.5f, 0.5f)); }
	static const Anchor BottomCenter()		{ return Anchor(mathfu::Vector<float, 2>(0.5f, 1.0f)); }
	static const Anchor TopRight()			{ return Anchor(mathfu::Vector<float, 2>(1.0f, 0.0f)); }
	static const Anchor CenterRight()		{ return Anchor(mathfu::Vector<float, 2>(1.0f, 0.5f)); }
	static const Anchor BottomRight()		{ return Anchor(mathfu::Vector<float, 2>(1.0f, 1.0f)); }

	mathfu::Vector<float, 2> normalizedValue;
};

typedef Anchor Alignment;

class BaseWidget
{
	CreateClassMetadata(BaseWidget)
public:
	template<typename MetaVarType, typename ...Args>
	void PopulateWidgetData(MetaVarType& metaVar, rapidjson::GenericArray<false, rapidjson::Value::ValueType>::PlainType* widgetData, Args&&... args)
	{
		rapidjson::Value::MemberIterator varMemberIterator = widgetData->FindMember(metaVar.GetName());
		if (varMemberIterator != widgetData->MemberEnd())
		{
			const auto& jsonVar = varMemberIterator->value;

			Variable vars[]{ args... };
			const unsigned int varArraySize = (sizeof(vars) / sizeof(Variable));

			if (varArraySize > 1)
			{
				const auto& jsonArray = jsonVar.GetArray();

				for (unsigned int i = 0; i < varArraySize; ++i)
				{
					CopyVariableFromJSON(vars[i], jsonArray[i]);
				}
			}
			else
			{
				CopyVariableFromJSON(vars[0], jsonVar);
			}
		}
	}

	virtual void Initialize(Renderer* renderer, rapidjson::GenericArray<false, rapidjson::Value::ValueType>::PlainType* widgetData);

	void AddChild(BaseWidget* newChild);
	void RemoveChild(BaseWidget* childToRemove);
	void RemoveFromParent();

	void SetParent(BaseWidget* inParent);
	BaseWidget* GetParent() { return parent; }

	BaseWidget* ProcessMouseInput(Input& mouseInput);
	virtual InputResponse TryHandleMousePress(mathfu::Vector<float, 2> mousePressPosition);

	virtual void Draw(Renderer* renderer, const SDL_Rect& parentRectBounds, bool isAnyParentDirty = false);

	void SetAnchor(Anchor newAnchor)
	{
		isTransformDirty = true;
		anchor = newAnchor;
		assert(abs(anchor.normalizedValue.x) <= 1);
		assert(abs(anchor.normalizedValue.y) <= 1);
	}

	void SetAlignment(Alignment newAlignment)
	{
		isTransformDirty = true;
		alignment = newAlignment;
		assert(abs(alignment.normalizedValue.x) <= 1);
		assert(abs(alignment.normalizedValue.y) <= 1);
	}

	virtual void SetWidthInLocalSpace(float inWidth)
	{
		isTransformDirty = true;
		boundsInLocalSpace.w = inWidth;
	}

	virtual void SetHeightInLocalSpace(float inHeight)
	{
		isTransformDirty = true;
		boundsInLocalSpace.h = inHeight;
	}

	virtual float GetWidthInLocalSpace() const
	{
		return boundsInLocalSpace.w;
	}

	virtual float GetHeightInLocalSpace() const
	{
		return boundsInLocalSpace.h;
	}

	SDL_Color GetColor() { return color; }

	void SetColor(SDL_Color inColor) { color = inColor; }

	virtual float GetWidthInGlobalSpace(const Renderer* const renderer, const SDL_Rect& parentRectBounds) const;
	virtual float GetHeightInGlobalSpace(const Renderer* const renderer, const SDL_Rect& parentRectBounds) const;

	void MousePressed(mathfu::Vector<float, 2> mousePressPosition);
	void MouseReleased(mathfu::Vector<float, 2> mouseReleasePosition);

	SingleCastDelegate<void>& GetOnMousePressedDel() { return onMousePressedDel; }
	SingleCastDelegate<void>& GetOnMouseReleasedDel() { return onMouseReleasedDel; }
protected:
	virtual SDL_Rect CalculateBoundsInGlobalSpace(const Renderer* const renderer, const SDL_Rect& parentRectBounds) const;

	virtual void OnMousePressed(mathfu::Vector<float, 2> mousePressPosition);
	virtual void OnMouseReleased(mathfu::Vector<float, 2> mouseReleasePosition);

	std::vector<BaseWidget*> children;

	BaseWidget* parent;
	char name[21];
	Anchor anchor;
	Alignment alignment;
	SDL_Rect boundsInLocalSpace;
	SDL_Rect cachedBoundsInGlobalSpace;

	SDL_Color color;

	SingleCastDelegate<void> onMousePressedDel;
	SingleCastDelegate<void> onMouseReleasedDel;
	
	bool isTransformDirty;

private:
	void CopyVariableFromJSON(Variable& destinationVar, const rapidjson::GenericArray<false, rapidjson::Value::ValueType>::PlainType& sourceJSON);
};