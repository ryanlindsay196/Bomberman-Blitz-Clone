#include "Engine/GameMode/GameMode.h"
#include "Game/GameManager.h"
#include "Engine/RCP.h"
#include "Game/EntityManager.h"

void MatchSetupPhase::OnPhaseStart()
{
	clientConnectionTimer = 1000;
}

void MatchSetupPhase::OnPhaseUpdate(float deltaTime)
{
	clientConnectionTimer -= deltaTime;
	printf("Time for players to connect: %f\n", clientConnectionTimer);
}

GamePhase * MatchSetupPhase::CheckForNextPhase()
{
	if (clientConnectionTimer <= 0)
	{
		return new MatchStartPhase();
	}
	return nullptr;
}

void MatchSetupPhase::OnPhaseEnd()
{
	unsigned int CreatePlayer_networkID = GameManager::GetNetworkManager().GenerateNewNetworkID();
	unsigned int CreatePlayer_owningClientID = 1;
	mathfu::Vector<float, 2> CreatePlayer_position{ 20.0f, 20.0f };
	mathfu::Vector<int, 2> CreatePlayer_textureDimensions{ 100, 100 };
	RPC::SendRpc(EntityManager::GetInstance(), "CreateEntity<Player>", CreatePlayer_networkID, CreatePlayer_owningClientID, CreatePlayer_position, CreatePlayer_textureDimensions);

	unsigned int CreateController_networkID = CreatePlayer_networkID;
	unsigned int CreateController_controllerID = GameManager::GetNetworkManager().GenerateNewNetworkID();
	unsigned int CreateController_owningClientID = 1;
	RPC::SendRpc(EntityManager::GetInstance(), "CreateController", CreateController_networkID, CreateController_controllerID, CreateController_owningClientID);

	CreatePlayer_networkID = GameManager::GetNetworkManager().GenerateNewNetworkID();
	CreatePlayer_position.x = 240.0f;
	CreatePlayer_position.y = 20.0;
	RPC::SendRpc(EntityManager::GetInstance(), "CreateEntity<Player>", CreatePlayer_networkID, CreatePlayer_owningClientID, CreatePlayer_position, CreatePlayer_textureDimensions);
	
	CreateController_networkID = CreatePlayer_networkID;
	CreateController_controllerID = GameManager::GetNetworkManager().GenerateNewNetworkID();
	CreateController_owningClientID++;
	RPC::SendRpc(EntityManager::GetInstance(), "CreateController", CreateController_networkID, CreateController_controllerID, CreateController_owningClientID);
	
	CreatePlayer_networkID = GameManager::GetNetworkManager().GenerateNewNetworkID();
	CreatePlayer_position.x = 20.0f;
	CreatePlayer_position.y = 240.0;
	RPC::SendRpc(EntityManager::GetInstance(), "CreateEntity<Player>", CreatePlayer_networkID, CreatePlayer_owningClientID, CreatePlayer_position, CreatePlayer_textureDimensions);
	
	CreateController_networkID = CreatePlayer_networkID;
	CreateController_controllerID = GameManager::GetNetworkManager().GenerateNewNetworkID();
	CreateController_owningClientID++;
	RPC::SendRpc(EntityManager::GetInstance(), "CreateController", CreateController_networkID, CreateController_controllerID, CreateController_owningClientID);
	
	CreatePlayer_networkID = GameManager::GetNetworkManager().GenerateNewNetworkID();
	CreatePlayer_position.x = 240.0f;
	CreatePlayer_position.y = 240.0;
	RPC::SendRpc(EntityManager::GetInstance(), "CreateEntity<Player>", CreatePlayer_networkID, CreatePlayer_owningClientID, CreatePlayer_position, CreatePlayer_textureDimensions);
	
	CreateController_networkID = CreatePlayer_networkID;
	CreateController_controllerID = GameManager::GetNetworkManager().GenerateNewNetworkID();
	CreateController_owningClientID++;
	RPC::SendRpc(EntityManager::GetInstance(), "CreateController", CreateController_networkID, CreateController_controllerID, CreateController_owningClientID);

}

void MatchStartPhase::OnPhaseStart()
{
	matchStartTimerInMilliseconds = 1000;
	//std::cout << "Starting match startup phase!\n";
}

void MatchStartPhase::OnPhaseUpdate(float deltaTime)
{
	printf("Time left until match start is: %f\n", matchStartTimerInMilliseconds / 100);
	matchStartTimerInMilliseconds -= deltaTime;
}

void MatchStartPhase::OnPhaseEnd()
{
	//std::cout << "Ending match startup phase!\n";
}

GamePhase * MatchStartPhase::CheckForNextPhase()
{
	if (MatchStartTimerExpired())
	{
		return new MatchInProgressPhase();
	}
	return nullptr;
}

void GameMode::StartGameMode(GamePhase * startingPhase)
{
	if (currentPhase)
	{
		return;
	}

	SetPhase(startingPhase);
}

void GameMode::Update(float deltaTime)
{
	currentPhase->OnPhaseUpdate(deltaTime);
	GamePhase* newPhase = currentPhase->CheckForNextPhase();
	if (newPhase)
	{
		SetPhase(newPhase);
	}
}

void GameMode::SetPhase(GamePhase * newPhase)
{
	if (!newPhase)
	{
		return;
	}

	if (currentPhase)
	{
		currentPhase->OnPhaseEnd();
	}

	if (currentPhase)
	{
		delete currentPhase;
	}

	currentPhase = newPhase;
	newPhase->OnPhaseStart();
}
