#include "Game/Player.h"
#include "Engine/Networking/NetworkManager.h"

void Player::Update(float deltaTime)
{
	Entity::Update(deltaTime);
}

bool Player::Initialize()
{
	health = 1;

	RegisterSelfAsNetworked();
	CreateVariableMetadata(Player, health, Networked(AuthorityType::Server, health))

	return Entity::Initialize();
}

void Player::Destroy()
{
}
