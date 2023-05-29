#include "Engine/GameInstances/ClientInstance.h"

bool ClientInstance::Initialize()
{
	if (isOpen)
	{
		return false;
	}

	rawPointerTracker.Initialize();
	networkManager.Initialize(false);
	renderer.Initialize(640, 480, false, "Bomberman");
	uiManager.Initialize(&renderer);
	entityManager.Initialize();

	Open();

	return true;
}

void ClientInstance::Update(float deltaTime)
{
	if (!isOpen)
	{
		return;
	}

	networkManager.Update(deltaTime);
	renderer.Update(deltaTime);
	uiManager.Update();

	entityManager.UpdateEntities(deltaTime);

	networkManager.SerializeNetworkedObjects();

	entityManager.RenderEntities();

	uiManager.Draw(&renderer);

	renderer.Render();
}

void ClientInstance::Open()
{
	isOpen = true;
}

void ClientInstance::Close()
{
	isOpen = false;
	renderer.Close();
}

void ClientInstance::Destroy()
{
	networkManager.Destroy();
	renderer.Destroy();

	//TODO: Quitting should happen on game manager destroy?
	SDL_Quit();
}
