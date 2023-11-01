#include "Engine/Engine.h"
#include "Game/GameManager.h"

void Engine::Initialize()
{
	CreateClassFunctionMetadata(Engine, PlayInEditor);
	
	renderer.Initialize(480, 640, false, "Blast Engine");
	uiManager.Initialize(&renderer, (char*)"Resources/UI/EngineUI.JSON");

	if (!GameManager::GetAssetManager()->GetIsInitialized())
	{
		GameManager::GetAssetManager()->InitializeAssetDatabase();
	}
}

bool Engine::Update(float deltaTime)
{
	GameManager::Get().Update(deltaTime);

	renderer.Update(deltaTime);
	uiManager.Update();
	uiManager.Draw(&renderer);

	renderer.Render();

	return renderer.IsOpen();
}

void Engine::PlayInEditor()
{
	GameManager::Get().Initialize();
}
