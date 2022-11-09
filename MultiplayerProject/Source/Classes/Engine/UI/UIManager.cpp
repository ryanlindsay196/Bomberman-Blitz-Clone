#include "Engine/UI/UIManager.h"
#include "Engine/UI/BaseWidget.h"

void UIManager::Initialize()
{
	tree.Initialize(1000000);

	//TODO: Remove this. This is just test code.
	BaseWidget* myWidget = tree.CreateWidget<BaseWidget>(nullptr);
	tree.CreateWidget<BaseWidget>(myWidget);
	tree.CreateWidget<BaseWidget>(myWidget);
	tree.CreateWidget<BaseWidget>(nullptr);

	for (int i = 0; i < 1000; ++i)
	{
		BaseWidget* removeThis = tree.CreateWidget<BaseWidget>(myWidget);
		tree.CreateWidget<BaseWidget>(myWidget);
		tree.RemoveWidget(removeThis);
	}
}

void UIManager::Update(float deltaTime)
{
}
