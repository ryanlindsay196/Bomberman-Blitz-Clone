#pragma once
#include <vector>
#include "mathfu/vector.h"
#include "SDL_rect.h"

#include "Engine/InputManager.h"

class Renderer;

enum class InputResponse
{
	Handled,
	UnHandled
};

class Anchor
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


	mathfu::Vector<float, 2> GetNormalizedValue() const { return normalizedValue; }
	
private:
	mathfu::Vector<float, 2> normalizedValue;
};

typedef Anchor Alignment;

class BaseWidget
{
public:
	virtual void Initialize(Renderer* renderer)
	{ 
		strcpy_s(name, sizeof(name) + 1, "12345678901234567890"); 
		isTransformDirty = true;
		color = SDL_Color{ 255,255,255,255 };
	}

	void AddChild(BaseWidget* newChild);
	void RemoveChild(BaseWidget* childToRemove);
	void RemoveFromParent();

	void SetParent(BaseWidget* inParent);
	BaseWidget* GetParent() { return parent; }

	bool ProcessMouseInput(Input& mouseInput);
	virtual void OnMousePressed(mathfu::Vector<float, 2> mousePressPosition);
	virtual InputResponse TryHandleMousePress(mathfu::Vector<float, 2> mousePressPosition);

	virtual void Draw(Renderer* renderer, const SDL_Rect& parentRectBounds, bool isAnyParentDirty = false);

	void SetAnchor(Anchor newAnchor)
	{
		isTransformDirty = true;
		anchor = newAnchor;
		assert(abs(anchor.GetNormalizedValue().x) <= 1);
		assert(abs(anchor.GetNormalizedValue().y) <= 1);
	}

	void SetAlignment(Alignment newAlignment)
	{
		isTransformDirty = true;
		alignment = newAlignment;
		assert(abs(alignment.GetNormalizedValue().x) <= 1);
		assert(abs(alignment.GetNormalizedValue().y) <= 1);
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

	virtual SDL_Rect CalculateBoundsInGlobalSpace(const Renderer* const renderer, const SDL_Rect& parentRectBounds) const;

	std::vector<BaseWidget*> children;
protected:

	BaseWidget* parent;
	char name[21];
	Anchor anchor;
	Alignment alignment;
	SDL_Rect boundsInLocalSpace;
	SDL_Rect cachedBoundsInGlobalSpace;

	SDL_Color color;
	
	bool isTransformDirty;
};