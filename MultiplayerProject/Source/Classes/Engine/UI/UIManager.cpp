#include "Engine/UI/UIManager.h"
#include "Engine/UI/ButtonWidget.h"

void UIManager::Initialize(Renderer* renderer)
{
	tree.Initialize(renderer, 1000000);

	//TODO: Remove this. This is just test code.
	Button* button = tree.CreateWidget<Button>(nullptr, renderer);
	button->SetWidthInLocalSpace(100);
	button->SetHeightInLocalSpace(100);
	button->SetAnchor(Anchor::TopLeft());
	button->SetAlignment(Alignment({ 0.5f, 0.5f }));

	button = tree.CreateWidget<Button>(nullptr, renderer);
	button->SetWidthInLocalSpace(100);
	button->SetHeightInLocalSpace(100);
	button->SetAnchor(Anchor::TopRight());
	button->SetAlignment(Alignment({ 0.5f, 0.5f }));

	button = tree.CreateWidget<Button>(nullptr, renderer);
	button->SetWidthInLocalSpace(100);
	button->SetHeightInLocalSpace(100);
	button->SetAnchor(Anchor::BottomLeft());
	button->SetAlignment(Alignment({ 0.5f, 0.5f }));

	button = tree.CreateWidget<Button>(nullptr, renderer);
	button->SetWidthInLocalSpace(100);
	button->SetHeightInLocalSpace(100);
	button->SetAnchor(Anchor::BottomRight());
	button->SetAlignment(Alignment({ 0.5f, 0.5f }));
	
	//On the center of the screen
	button = tree.CreateWidget<Button>(nullptr, renderer);
	button->SetWidthInLocalSpace(640);
	button->SetHeightInLocalSpace(1);
	button->SetAnchor(Anchor::CenterCenter());
	button->SetAlignment(Alignment({ 0.5f, 0.5f }));

	button = tree.CreateWidget<Button>(nullptr, renderer);
	button->SetWidthInLocalSpace(1);
	button->SetHeightInLocalSpace(480);
	button->SetAnchor(Anchor::CenterCenter());
	button->SetAlignment(Alignment({ 0.5f, 0.5f }));

	//button = tree.CreateWidget<Image>(nullptr, renderer);
	//button->SetWidthInLocalSpace(50);
	//button->SetHeightInLocalSpace(400);
	//button->SetAnchor(Anchor::CenterCenter());
	//button->SetAlignment(Alignment({ 0.5f, 0.5f }));

	//Image* childImage = tree.CreateWidget<Image>(button, renderer);
	//childImage->SetWidthInLocalSpace(200);
	//childImage->SetHeightInLocalSpace(200);
	//childImage->SetAnchor(Anchor({ 0.25f, 0.25f }));
	//childImage->SetAlignment(Alignment({ 0.0f, 0.0f }));
}

void UIManager::Update()
{
	tree.ProcessInput();
}

void UIManager::Draw(Renderer* renderer)
{
	tree.Draw(renderer);
}
