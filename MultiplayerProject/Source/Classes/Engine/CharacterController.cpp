#include "Engine/CharacterController.h"
#include "Game/GameManager.h"

bool CharacterController::Initialize()
{
	RegisterSelfAsNetworked();
	CreateVariableMetadata(CharacterController, leftAxis, Networked(AuthorityType::OwningClient, leftAxis))
	CreateVariableMetadata(CharacterController, rightAxis, Networked(AuthorityType::OwningClient, rightAxis))

	leftAxis = mathfu::Vector<float, 2>{ 0,0 };
	rightAxis = mathfu::Vector<float, 2>{ 0,0 };
	
	return true;
}

void CharacterController::UpdateInputs()
{
	if (GetOwningClientID() != GameManager::GetNetworkManager().GetClientID())
		return;

	leftAxis = mathfu::Vector<float, 2>(0, 0);
	rightAxis = mathfu::Vector<float, 2>(0, 0);

	GameManager& gameManager = GameManager::Get();
	if (gameManager.GetInputManager()->IsKeyDown(SDLK_UP, true))
	{
		leftAxis.y -= 1;
	}
	if (gameManager.GetInputManager()->IsKeyDown(SDLK_DOWN, true))
	{
		leftAxis.y += 1;
	}
	if (gameManager.GetInputManager()->IsKeyDown(SDLK_RIGHT, true))
	{
		leftAxis.x += 1;
	}
	if (gameManager.GetInputManager()->IsKeyDown(SDLK_LEFT, true))
	{
		leftAxis.x -= 1;
	}
}
