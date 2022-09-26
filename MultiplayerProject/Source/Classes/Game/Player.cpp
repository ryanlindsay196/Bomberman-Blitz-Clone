#include "Game/Player.h"
#include "Engine/Networking/NetworkManager.h"

void Player::Update(float deltaTime)
{
	Entity::Update(deltaTime);
}

bool Player::Initialize(const char* imagePath, float inWidth, float inHeight, float inPosX, float inPosY)
{
	health = 1;

	RegisterSelfAsNetworked();
	CreateVariableMetadata(Player, health, Networked(AuthorityType::Server, health))

	controller = new CharacterController();
	controller->Initialize(nullptr, 0, 0, 0, 0);

	return Entity::Initialize(imagePath, inWidth, inHeight, inPosX, inPosY);
}

void Player::Destroy()
{
	delete(controller);
}
