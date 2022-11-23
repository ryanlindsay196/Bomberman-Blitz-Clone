#pragma once

#include "Engine/GameInstances/GameInstance.h"

class ClientInstance : public GameInstance
{
public:
	virtual bool Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void Open() override;
	virtual void Close() override;
	virtual void Destroy() override;

	virtual NetworkManager& GetNetworkManager() override { return networkManager; }
	virtual Renderer* GetRenderer() override { return &renderer; }
	virtual EntityManager* GetEntityManager() override { return &entityManager; }
	virtual TextureManager* GetTextureManager() override { return &textureManager; }

private:
	Renderer renderer;
	NetworkManager networkManager;
	EntityManager entityManager;
	TextureManager textureManager;
	UIManager uiManager;

	bool isOpen;
};