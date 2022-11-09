#pragma once
#include "Engine/UI/WidgetTree.h"

class UIManager
{
public:
	void Initialize();
	void Update(float deltaTime);

private:
	WidgetTree tree;
};