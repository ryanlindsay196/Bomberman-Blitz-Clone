#include "Engine/UI/WidgetTree.h"
#include "Engine/Rendering/Renderer.h"
#include "Engine/Managers/AssetManager.h"
#include "Game/GameManager.h"

void WidgetTree::Initialize(Renderer* renderer, size_t widgetAllocatorSizeInBytes)
{
	allocator.Init(widgetAllocatorSizeInBytes);
	rootWidget.Initialize(renderer, nullptr);
	rootWidget.SetWidthInLocalSpace(renderer->GetViewportWidth());
	rootWidget.SetHeightInLocalSpace(renderer->GetViewportHeight());
}

BaseWidget* WidgetTree::CreateWidget(rapidjson::Value* currentNode, BaseWidget * parentWidget, Renderer * renderer)
{
	if (!renderer)
	{
		return nullptr;
	}

	auto jsonArray = currentNode->GetArray();
	BaseWidget* newWidget = nullptr;

	for (unsigned int i = 0; i < jsonArray.Size(); ++i)
	{
		auto& jsonArrayElement = jsonArray[i];
		auto& jsonClassName = jsonArrayElement["class"];

		char* desiredClassName = const_cast<char*>(jsonClassName.GetString());

		unsigned int sizeToAllocate = GameManager::GetAssetManager()->GetAssetSizeByName(desiredClassName);
		void* newPtr = allocator.Alloc(sizeToAllocate, 8);
		newWidget = (BaseWidget*)GameManager::GetAssetManager()->InstantiateByName(desiredClassName, newPtr);
		newWidget->Initialize(renderer, &jsonArrayElement);

		if (parentWidget)
		{
			parentWidget->AddChild(newWidget);
			newWidget->SetParent(parentWidget);
		}
		else
		{
			rootWidget.AddChild(newWidget);
			newWidget->SetParent(nullptr);
		}

		rapidjson::Value::MemberIterator child = jsonArrayElement.FindMember("Children");
		if(child != jsonArrayElement.MemberEnd())
		{
			CreateWidget(&child->value, newWidget, renderer);
		}
	}
	assert(newWidget);
	return newWidget;
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
		if (GameManager::GetInputManager()->IsMouseButtonReleased(SDL_BUTTON_LEFT, false) || !GameManager::IsUpdatingFocusedWindow())
		{
			Input& mouseInput = GameManager::GetInputManager()->GetInputByMouseButtonID(SDL_BUTTON_LEFT);
			mouseInput.isConsumed = true;

			mathfu::Vector<int, 2> mousePosition(mouseInput.e.button.x, mouseInput.e.button.y);
			clickedWidget->MouseReleased(mousePosition);
			clickedWidget = nullptr;
		}
	}
}
