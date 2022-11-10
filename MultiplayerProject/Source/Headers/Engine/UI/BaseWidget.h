#pragma once
#include <vector>
#include "mathfu/vector.h"

class Renderer;

class Anchor
{
	static const mathfu::Vector<float, 2> TopLeft()			{ return { 0.0f, 0.0f }; }
	static const mathfu::Vector<float, 2> CenterLeft()		{ return { 0.0f, 0.5f }; }
	static const mathfu::Vector<float, 2> BottomLeft()		{ return { 0.0f, 1.0f }; }
	static const mathfu::Vector<float, 2> TopCenter()		{ return { 0.5f, 0.0f }; }
	static const mathfu::Vector<float, 2> CenterCenter()	{ return { 0.5f, 0.5f }; }
	static const mathfu::Vector<float, 2> BottomCenter()	{ return { 0.5f, 1.0f }; }
	static const mathfu::Vector<float, 2> TopRight()		{ return { 1.0f, 0.0f }; }
	static const mathfu::Vector<float, 2> CenterRight()		{ return { 1.0f, 0.5f }; }
	static const mathfu::Vector<float, 2> BottomRight()		{ return { 1.0f, 1.0f }; }
	mathfu::Vector<float, 2> normalizedAnchor;
};

class BaseWidget
{
public:
	virtual void Initialize(Renderer* renderer){ strcpy_s(name, sizeof(name) + 1, "12345678901234567890"); }

	void AddChild(BaseWidget* newChild);
	void RemoveChild(BaseWidget* childToRemove);
	void RemoveFromParent();

	void SetParent(BaseWidget* inParent);
	BaseWidget* GetParent() { return parent; }
	virtual void Draw(Renderer* renderer);

	std::vector<BaseWidget*> children;
private:

	BaseWidget* parent;
	char name[21];
	Anchor anchor;
	mathfu::Vector<float, 2> offset;
};