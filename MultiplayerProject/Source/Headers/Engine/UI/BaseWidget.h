#pragma once
#include <vector>

class BaseWidget
{
public:
	BaseWidget() 
	{
		strcpy_s(name, sizeof(name) + 1, "12345678901234567890");
	}

	void AddChild(BaseWidget* newChild);
	void RemoveChild(BaseWidget* childToRemove);
	void RemoveFromParent();

	void SetParent(BaseWidget* inParent);
	BaseWidget* GetParent() { return parent; }
	void Draw();

	std::vector<BaseWidget*> children;
private:

	BaseWidget* parent;
	char name[20];
};