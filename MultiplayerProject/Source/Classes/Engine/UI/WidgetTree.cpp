#include "Engine/UI/WidgetTree.h"
#include "Engine/Rendering/Renderer.h"
#include "Game/GameManager.h"

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

void WidgetTree::ProcessInput()
{
	if (GameManager::Get().GetInputManager()->IsMouseButtonPressed(SDL_BUTTON_LEFT, false))
	{
		Input& mouseInput = GameManager::Get().GetInputManager()->GetInputByMouseButtonID(SDL_BUTTON_LEFT);
		if(rootWidget.ProcessMouseInput(mouseInput))
		{
			mouseInput.isConsumed = true;
		}
	}

	//TODO: OnMouseMoved

	//TODO: OnMouseReleased
}
