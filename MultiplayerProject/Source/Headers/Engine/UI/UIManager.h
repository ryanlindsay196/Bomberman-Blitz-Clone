#pragma once
#include "Engine/UI/WidgetTree.h"

class UIManager
{
public:
	void Initialize(Renderer* renderer);
	void Update(Renderer* renderer);

private:
	WidgetTree tree;
};