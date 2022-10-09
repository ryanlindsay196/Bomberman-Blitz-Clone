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
	unsigned int CreatePlayer_arg0 = GameManager::GetNetworkManager().GenerateNewNetworkID();
	mathfu::Vector<float, 2> CreatePlayer_arg1{ 20.0f, 20.0f };
	mathfu::Vector<int, 2> CreatePlayer_arg2{ 100, 100 };
	RPC::SendRpc(EntityManager::GetInstance(), "CreateEntity<Player>", CreatePlayer_arg0, CreatePlayer_arg1, CreatePlayer_arg2);

	unsigned int CreateController_arg0 = CreatePlayer_arg0;
	unsigned int CreateController_arg1 = GameManager::GetNetworkManager().GenerateNewNetworkID();
	RPC::SendRpc(EntityManager::GetInstance(), "CreateController", CreateController_arg0, CreateController_arg1);

	CreatePlayer_arg0 = GameManager::GetNetworkManager().GenerateNewNetworkID();
	CreatePlayer_arg1.x = 240.0f;
	CreatePlayer_arg1.y = 20.0;
	RPC::SendRpc(EntityManager::GetInstance(), "CreateEntity<Player>", CreatePlayer_arg0, CreatePlayer_arg1, CreatePlayer_arg2);

	CreateController_arg0 = CreatePlayer_arg0;
	CreateController_arg1 = GameManager::GetNetworkManager().GenerateNewNetworkID();
	RPC::SendRpc(EntityManager::GetInstance(), "CreateController", CreateController_arg0, CreateController_arg1);

	CreatePlayer_arg0 = GameManager::GetNetworkManager().GenerateNewNetworkID();
	CreatePlayer_arg1.x = 20.0f;
	CreatePlayer_arg1.y = 240.0;
	RPC::SendRpc(EntityManager::GetInstance(), "CreateEntity<Player>", CreatePlayer_arg0, CreatePlayer_arg1, CreatePlayer_arg2);
	
	CreateController_arg0 = CreatePlayer_arg0;
	CreateController_arg1 = GameManager::GetNetworkManager().GenerateNewNetworkID();
	RPC::SendRpc(EntityManager::GetInstance(), "CreateController", CreateController_arg0, CreateController_arg1);

	CreatePlayer_arg0 = GameManager::GetNetworkManager().GenerateNewNetworkID();
	CreatePlayer_arg1.x = 240.0f;
	CreatePlayer_arg1.y = 240.0;
	RPC::SendRpc(EntityManager::GetInstance(), "CreateEntity<Player>", CreatePlayer_arg0, CreatePlayer_arg1, CreatePlayer_arg2);
	
	CreateController_arg0 = CreatePlayer_arg0;
	CreateController_arg1 = GameManager::GetNetworkManager().GenerateNewNetworkID();
	RPC::SendRpc(EntityManager::GetInstance(), "CreateController", CreateController_arg0, CreateController_arg1);

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
