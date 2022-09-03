#include "Game/GameManager.h"

int main(int argc, char* args[])
{
	bool isInGameLoop = true;
	
	GameManager& gameManager = GameManager::Get();
	gameManager.Initialize();
	
	while (isInGameLoop)
	{
		isInGameLoop = gameManager.Update();
	}
	
	gameManager.Destroy();

	return 0;
}
