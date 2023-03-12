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
	ProcessMouseClicked();

	//TODO: OnMouseMoved

	ProcessMouseReleased();

}

void WidgetTree::ProcessMouseClicked()
{
	if (GameManager::GetInputManager()->IsMouseButtonPressed(SDL_BUTTON_LEFT, false))
	{
		Input& mouseInput = GameManager::GetInputManager()->GetInputByMouseButtonID(SDL_BUTTON_LEFT);
		clickedWidget = rootWidget.ProcessMouseInput(mouseInput);
		if (clickedWidget)
		{
			mouseInput.isConsumed = true;
		}
	}
}

void WidgetTree::ProcessMouseReleased()
{
	if (clickedWidget)
	{
		if (GameManager::GetInputManager()->IsMouseButtonReleased(SDL_BUTTON_LEFT, false))
		{
			Input& mouseInput = GameManager::GetInputManager()->GetInputByMouseButtonID(SDL_BUTTON_LEFT);
			mouseInput.isConsumed = true;

			mathfu::Vector<float, 2> mousePosition(mouseInput.e.button.x, mouseInput.e.button.y);
			clickedWidget->OnMouseReleased(mousePosition);
			clickedWidget = nullptr;
		}
	}
}
