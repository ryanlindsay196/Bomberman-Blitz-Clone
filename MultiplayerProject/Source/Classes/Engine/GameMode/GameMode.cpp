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
	unsigned int arg0 = GameManager::GetNetworkManager().GenerateNewNetworkID();
	mathfu::Vector<float, 2> arg1{ 20.0f, 20.0f };
	mathfu::Vector<int, 2> arg2{ 100, 100 };

	Variable vars[3]{ arg0, arg1, arg2 };

	//TODO: Don't do GetMetaFunctionByID. Instead, get it by signature or function name.
	RPC::SendRpc(EntityManager::GetInstance(), EntityManager::GetInstance()->GetMetaFunctionByID(1), arg0, arg1, arg2);

	arg0 = GameManager::GetNetworkManager().GenerateNewNetworkID();
	arg1.x = 240.0f;
	arg1.y = 20.0;
	RPC::SendRpc(EntityManager::GetInstance(), EntityManager::GetInstance()->GetMetaFunctionByID(1), arg0, arg1, arg2);

	arg0 = GameManager::GetNetworkManager().GenerateNewNetworkID();
	arg1.x = 20.0f;
	arg1.y = 240.0;
	RPC::SendRpc(EntityManager::GetInstance(), EntityManager::GetInstance()->GetMetaFunctionByID(1), arg0, arg1, arg2);

	arg0 = GameManager::GetNetworkManager().GenerateNewNetworkID();
	arg1.x = 240.0f;
	arg1.y = 240.0;
	RPC::SendRpc(EntityManager::GetInstance(), EntityManager::GetInstance()->GetMetaFunctionByID(1), arg0, arg1, arg2);
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
