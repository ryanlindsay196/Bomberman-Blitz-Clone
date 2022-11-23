#pragma once

#include "Engine/GameInstances/GameInstance.h"

class ServerInstance : public GameInstance
{
public:
	virtual bool Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void Open() override;
	virtual void Close() override;
	virtual void Destroy() override;

	virtual NetworkManager& GetNetworkManager() override { return networkManager; }
	virtual Renderer* GetRenderer() override { return nullptr; }
	virtual EntityManager* GetEntityManager() override { return &entityManager; }
	virtual TextureManager* GetTextureManager() override { return nullptr; }

private:
	NetworkManager networkManager;
	EntityManager entityManager;

	bool isOpen;
};