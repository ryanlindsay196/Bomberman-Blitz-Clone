#include "Engine/UI/WidgetTree.h"
#include "Engine/Rendering/Renderer.h"
#include "Game/GameManager.h"

void WidgetTree::Initialize(Renderer* renderer, size_t widgetAllocatorSizeInBytes)
{
	allocator.Init(widgetAllocatorSizeInBytes);
	rootWidget.Initialize(renderer);
	rootWidget.SetWidthInLocalSpace(renderer->GetViewportWidth());
	rootWidget.SetHeightInLocalSpace(renderer->GetViewportHeight());
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
	//SDL_Rect viewportBounds{ 0, 0, 640, 480 };
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
