#include "Engine/UI/UIManager.h"
#include "Engine/UI/ImageWidget.h"

void UIManager::Initialize(Renderer* renderer)
{
	tree.Initialize(1000000);

	//TODO: Remove this. This is just test code.
	Image* myWidget = tree.CreateWidget<Image>(nullptr, renderer);
	tree.CreateWidget<Image>(myWidget, renderer);
	tree.CreateWidget<Image>(myWidget, renderer);
	tree.CreateWidget<Image>(nullptr, renderer);

	//for (int i = 0; i < 1000; ++i)
	//{
	//	Image* removeThis = tree.CreateWidget<Image>(myWidget);
	//	tree.CreateWidget<Image>(myWidget);
	//	tree.RemoveWidget(removeThis);
	//}
}

void UIManager::Update(Renderer* renderer)
{
	tree.Draw(renderer);
}
