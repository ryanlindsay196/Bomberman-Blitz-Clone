#include "Engine/Engine.h"
#include "Game/GameManager.h"

void Engine::Initialize()
{
	renderer.Initialize(480, 640, false, "Blast Engine");
	uiManager.Initialize(&renderer, (char*)"Resources/UI/PlayerHud.JSON");

	if (!GameManager::GetAssetManager()->GetIsInitialized())
	{
		GameManager::GetAssetManager()->InitializeAssetDatabase();
	}

	//TODO: Make the PlayInEditor function a specific button rather than clicking on the root widget.
	uiManager.GetRootWidget()->GetOnMousePressedDel().BindMemberFunction(this, &Engine::PlayInEditor);
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
