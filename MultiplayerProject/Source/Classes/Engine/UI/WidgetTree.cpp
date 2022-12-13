#include "Engine/UI/WidgetTree.h"
#include "Engine/Rendering/Renderer.h"

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

void WidgetTree::Draw(Renderer * renderer)
{
	SDL_Rect viewportBounds{ 0, 0, renderer->GetViewportWidth(), renderer->GetViewportHeight() };
	rootWidget.Draw(renderer, viewportBounds);
}
