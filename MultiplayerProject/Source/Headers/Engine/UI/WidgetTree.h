#pragma once

#include "Engine/UI/BaseWidget.h"
#include "Engine/Allocators/FreeListAllocator.h"

struct TEMPUINode
{
	unsigned int ID;
	std::vector<TEMPUINode*> children;
};

class WidgetTree
{
public:
	void Initialize(Renderer* renderer, size_t widgetAllocatorSizeInBytes);

	BaseWidget* CreateWidget(TEMPUINode* currentNode, BaseWidget* parentWidget, Renderer* renderer);

	void RemoveWidget(BaseWidget* widgetToRemove);

	void Draw(Renderer* renderer);

	void ProcessInput();

	BaseWidget* GetRootWidget() { return &rootWidget; }

private:
	void ProcessMouseClicked();
	void ProcessMouseReleased();

	BaseWidget rootWidget;
	FreeListAllocator allocator;

	BaseWidget* clickedWidget;
};