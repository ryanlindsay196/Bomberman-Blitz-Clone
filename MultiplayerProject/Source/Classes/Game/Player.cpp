#include "Game/Player.h"
#include "Engine/Networking/NetworkManager.h"

void Player::Update(float deltaTime)
{
	GameManager& gameManager = GameManager::Get();
	if (gameManager.GetInputManager()->IsKeyDown(SDLK_UP, true))
	{
		posY -= deltaTime;
	}
	if (gameManager.GetInputManager()->IsKeyDown(SDLK_DOWN, true))
	{
		posY += deltaTime;
	}
	if (gameManager.GetInputManager()->IsKeyDown(SDLK_RIGHT, true))
	{
		posX += deltaTime;
	}
	if (gameManager.GetInputManager()->IsKeyDown(SDLK_LEFT, true))
	{
		posX -= deltaTime;
	}

	Entity::Update(deltaTime);
}

bool Player::Initialize(const char* imagePath, float inWidth, float inHeight, float inPosX, float inPosY)
{
	health = 1;
	RegisterNetworkedVariable(Player, health, AuthorityType::Server);

	return Entity::Initialize(imagePath, inWidth, inHeight, inPosX, inPosY);
}
