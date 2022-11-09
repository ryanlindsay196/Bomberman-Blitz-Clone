#include "Engine/UI/WidgetTree.h"

void WidgetTree::Initialize(size_t widgetAllocatorSizeInBytes)
{
	allocator.Init(widgetAllocatorSizeInBytes);
}

void WidgetTree::RemoveWidget(BaseWidget * widgetToRemove)
{
	if (!widgetToRemove)
	{
		return;
	}

	widgetToRemove->RemoveFromParent();

	allocator.Free(widgetToRemove);
}
