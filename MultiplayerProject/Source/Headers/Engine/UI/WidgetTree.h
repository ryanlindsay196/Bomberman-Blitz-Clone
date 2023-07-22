#pragma once

#include "Engine/UI/BaseWidget.h"
#include "Engine/Allocators/FreeListAllocator.h"
#include "rapidjson/document.h"

class WidgetTree
{
public:
	void Initialize(Renderer* renderer, size_t widgetAllocatorSizeInBytes);

	BaseWidget* CreateWidget(rapidjson::Value* currentNode, BaseWidget* parentWidget, Renderer* renderer);

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