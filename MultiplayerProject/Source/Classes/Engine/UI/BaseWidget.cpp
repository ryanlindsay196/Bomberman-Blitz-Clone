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

void BaseWidget::Draw(Renderer* renderer)
{
	for (BaseWidget* child : children)
	{
		if (child)
		{
			child->Draw(renderer);
		}
	}
}
