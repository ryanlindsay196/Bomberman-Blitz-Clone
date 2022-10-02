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

	controller = new CharacterController();
	controller->Initialize();

	return Entity::Initialize();
}

void Player::Destroy()
{
	delete(controller);
}
