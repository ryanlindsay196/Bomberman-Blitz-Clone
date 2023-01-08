#pragma once
#include <vector>
#include "mathfu/vector.h"
#include "SDL_rect.h"

class Renderer;

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
	virtual void Initialize(Renderer* renderer){ strcpy_s(name, sizeof(name) + 1, "12345678901234567890"); }

	void AddChild(BaseWidget* newChild);
	void RemoveChild(BaseWidget* childToRemove);
	void RemoveFromParent();

	void SetParent(BaseWidget* inParent);
	BaseWidget* GetParent() { return parent; }
	virtual void Draw(Renderer* renderer, const SDL_Rect& parentRectBounds);

	void SetAnchor(Anchor newAnchor)
	{
		anchor = newAnchor;
		assert(abs(anchor.GetNormalizedValue().x) <= 1);
		assert(abs(anchor.GetNormalizedValue().y) <= 1);
	}

	void SetAlignment(Alignment newAlignment)
	{
		alignment = newAlignment;
		assert(abs(alignment.GetNormalizedValue().x) <= 1);
		assert(abs(alignment.GetNormalizedValue().y) <= 1);
	}

	virtual void SetWidthInLocalSpace(float inWidth)
	{
		widthInLocalSpace = inWidth;
	}

	virtual void SetHeightInLocalSpace(float inHeight)
	{ 
		heightInLocalSpace = inHeight;
	}

	virtual float GetWidthInLocalSpace() const
	{
		return widthInLocalSpace;
	}

	virtual float GetHeightInLocalSpace() const
	{
		return heightInLocalSpace;
	}

	virtual float GetWidthInGlobalSpace(const Renderer* const renderer, const SDL_Rect& parentRectBounds) const;
	virtual float GetHeightInGlobalSpace(const Renderer* const renderer, const SDL_Rect& parentRectBounds) const;

	virtual SDL_Rect CalculateBoundsInGlobalSpace(const Renderer* const renderer, const SDL_Rect& parentRectBounds) const;

	std::vector<BaseWidget*> children;
protected:

	BaseWidget* parent;
	char name[21];
	Anchor anchor;
	Alignment alignment;
	float widthInLocalSpace, heightInLocalSpace;
};