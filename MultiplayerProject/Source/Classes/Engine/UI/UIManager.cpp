#include "Engine/UI/UIManager.h"
#include "Engine/UI/ImageWidget.h"

void UIManager::Initialize(Renderer* renderer)
{
	tree.Initialize(1000000);

	//TODO: Remove this. This is just test code.
	Image* myWidget = tree.CreateWidget<Image>(nullptr, renderer);
	Image* image = tree.CreateWidget<Image>(myWidget, renderer);
	image->SetWidth(100);
	image->SetHeight(100);
	image->SetAnchor(Anchor::TopLeft());
	image->SetAlignment(Alignment({ 0.5f, 0.5f }));
	image->SetOffset({ 0.0f, 0.0f });

	image = tree.CreateWidget<Image>(myWidget, renderer);
	image->SetWidth(100);
	image->SetHeight(100);
	image->SetAnchor(Anchor::TopRight());
	image->SetAlignment(Alignment({ 0.5f, 0.5f }));
	image->SetOffset({ 0.0f, 0.0f });

	image = tree.CreateWidget<Image>(myWidget, renderer);
	image->SetWidth(100);
	image->SetHeight(100);
	image->SetAnchor(Anchor::BottomLeft());
	image->SetAlignment(Alignment({ 0.5f, 0.5f }));
	image->SetOffset({ 0.0f, 0.0f });

	image = tree.CreateWidget<Image>(myWidget, renderer);
	image->SetWidth(100);
	image->SetHeight(100);
	image->SetAnchor(Anchor::BottomRight());
	image->SetAlignment(Alignment({ 0.5f, 0.5f }));
	image->SetOffset({ 0.0f, 0.0f });

	//On the center of the screen
	//image = tree.CreateWidget<Image>(myWidget, renderer);
	//image->SetWidth(100)->SetHeight(100)->
	//	SetAnchor(Anchor::CenterCenter())->
	//	SetAlignment(Alignment({ 0.5f, 0.5f }))->
	//	SetOffset({ 0.0f, 0.0f });
	//
	//Slighty offset from the center of the screen
	//image = tree.CreateWidget<Image>(myWidget, renderer);
	//image->SetWidth(100)->SetHeight(100)->
	//	SetAnchor(Anchor({0.5f, 0.5f}))->
	//	SetAlignment(Alignment({ 1.0f, 0.5f }))->
	//	SetOffset({ 0.0f, 0.1f });

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
