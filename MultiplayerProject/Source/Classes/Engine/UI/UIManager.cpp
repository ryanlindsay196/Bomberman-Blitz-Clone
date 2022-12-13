#include "Engine/UI/UIManager.h"
#include "Engine/UI/ImageWidget.h"

void UIManager::Initialize(Renderer* renderer)
{
	tree.Initialize(1000000);

	//TODO: Remove this. This is just test code.
	Image* image = tree.CreateWidget<Image>(nullptr, renderer);
	image->SetWidthInLocalSpace(100);
	image->SetHeightInLocalSpace(100);
	image->SetAnchor(Anchor::TopLeft());
	image->SetAlignment(Alignment({ 0.5f, 0.5f }));

	image = tree.CreateWidget<Image>(nullptr, renderer);
	image->SetWidthInLocalSpace(100);
	image->SetHeightInLocalSpace(100);
	image->SetAnchor(Anchor::TopRight());
	image->SetAlignment(Alignment({ 0.5f, 0.5f }));

	image = tree.CreateWidget<Image>(nullptr, renderer);
	image->SetWidthInLocalSpace(100);
	image->SetHeightInLocalSpace(100);
	image->SetAnchor(Anchor::BottomLeft());
	image->SetAlignment(Alignment({ 0.5f, 0.5f }));

	image = tree.CreateWidget<Image>(nullptr, renderer);
	image->SetWidthInLocalSpace(100);
	image->SetHeightInLocalSpace(100);
	image->SetAnchor(Anchor::BottomRight());
	image->SetAlignment(Alignment({ 0.5f, 0.5f }));
	
	//On the center of the screen
	image = tree.CreateWidget<Image>(nullptr, renderer);
	image->SetWidthInLocalSpace(640);
	image->SetHeightInLocalSpace(1);
	image->SetAnchor(Anchor::CenterCenter());
	image->SetAlignment(Alignment({ 0.5f, 0.5f }));

	image = tree.CreateWidget<Image>(nullptr, renderer);
	image->SetWidthInLocalSpace(1);
	image->SetHeightInLocalSpace(480);
	image->SetAnchor(Anchor::CenterCenter());
	image->SetAlignment(Alignment({ 0.5f, 0.5f }));

	//image = tree.CreateWidget<Image>(nullptr, renderer);
	//image->SetWidthInLocalSpace(50);
	//image->SetHeightInLocalSpace(400);
	//image->SetAnchor(Anchor::CenterCenter());
	//image->SetAlignment(Alignment({ 0.5f, 0.5f }));

	//Image* childImage = tree.CreateWidget<Image>(image, renderer);
	//childImage->SetWidthInLocalSpace(200);
	//childImage->SetHeightInLocalSpace(200);
	//childImage->SetAnchor(Anchor({ 0.25f, 0.25f }));
	//childImage->SetAlignment(Alignment({ 0.0f, 0.0f }));
}

void UIManager::Update(Renderer* renderer)
{
	tree.Draw(renderer);
}
