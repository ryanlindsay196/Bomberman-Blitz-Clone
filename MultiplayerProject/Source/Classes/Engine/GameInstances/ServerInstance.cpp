#include "Engine/GameInstances/ServerInstance.h"

bool ServerInstance::Initialize()
{
	if (isOpen)
	{
		return false;
	}

	rawPointerTracker.Initialize();
	networkManager.Initialize(true);
	entityManager.Initialize();

	Open();

	return true;
}

void ServerInstance::Update(float deltaTime)
{
	if (!isOpen)
	{
		return;
	}

	networkManager.Update(deltaTime);

	entityManager.UpdateEntities(deltaTime);

	networkManager.SerializeNetworkedObjects();
}

void ServerInstance::Open()
{
	isOpen = true;
}

void ServerInstance::Close()
{
	isOpen = false;
}

void ServerInstance::Destroy()
{
	networkManager.Destroy();
}
