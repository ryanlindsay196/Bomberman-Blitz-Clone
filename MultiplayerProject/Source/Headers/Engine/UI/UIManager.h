#pragma once
#include "Engine/UI/WidgetTree.h"

class UIManager
{
public:
	void Initialize(Renderer* renderer, char* uiFile);
	void Update();
	void Draw(Renderer* renderer);


	BaseWidget* GetRootWidget() { return tree.GetRootWidget(); }
private:
	WidgetTree tree;
};