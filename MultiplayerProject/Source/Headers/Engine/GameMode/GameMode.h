#pragma once

class GamePhase
{
public:
	virtual void OnPhaseStart() {}
	virtual void OnPhaseUpdate(float deltaTime) {}
	virtual void OnPhaseEnd() {}

	virtual GamePhase* CheckForNextPhase() { return nullptr; }
};

class MatchSetupPhase : public GamePhase
{
	virtual void OnPhaseStart() override;
	
	virtual void OnPhaseUpdate(float deltaTime) override;

	virtual GamePhase* CheckForNextPhase();

	virtual void OnPhaseEnd();

	float clientConnectionTimer;
};

class MatchInProgressPhase : public GamePhase
{
	//Ends when one player is alive
};

class MatchStartPhase : public GamePhase
{
public:
	virtual void OnPhaseStart() override;

	virtual void OnPhaseUpdate(float deltaTime) override;

	virtual void OnPhaseEnd() override;

	virtual GamePhase* CheckForNextPhase() override;

	//Ends when we have a full lobby or after a certain time limit.
	//When we end, fill the lobby with CPUs.
private:
	float matchStartTimerInMilliseconds;

	bool MatchStartTimerExpired() { return matchStartTimerInMilliseconds <= 0; }
};

class MatchEndPhase : public GamePhase
{

};

class GameMode
{
public:
	void StartGameMode(GamePhase* startingPhase);

	void Update(float deltaTime);

protected:
	void SetPhase(GamePhase* newPhase);

	GamePhase* currentPhase = nullptr;
};