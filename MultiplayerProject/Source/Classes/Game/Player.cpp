#include "Game/Player.h"
#include "Engine/Networking/NetworkManager.h"

CreateClassMetadata(Player);

void Player::Update(float deltaTime)
{
	Entity::Update(deltaTime);
	spriteSheet.Update(deltaTime);
}

bool Player::Initialize()
{
	health = 1;

	RegisterSelfAsNetworked();
	CreateVariableMetadata(Player, health, Networked(AuthorityType::Server, health))

	spriteSheet.Initialize("Resources/SpriteSheets/PlayerSpriteSheet.JSON");

	return Entity::Initialize();
}

void Player::Destroy()
{
}
