#pragma once

#include "Engine/Rendering/Renderer.h"
#include "Engine/InputManager.h"
#include "Engine/Networking/NetworkManager.h"
#include "Engine/Managers/TextureManager.h"
#include "Engine/UI/UIManager.h"
#include "Game/EntityManager.h"
#include "Engine/RawPointerTracker.h"

class GameInstance
{
public:
	virtual bool Initialize() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Open() = 0;
	virtual void Close() = 0;
	virtual void Destroy() = 0;

	virtual NetworkManager& GetNetworkManager() = 0;
	virtual Renderer* GetRenderer() = 0;
	virtual EntityManager* GetEntityManager() = 0;
	virtual TextureManager* GetTextureManager() = 0;
	virtual UIManager* GetUIManager() { return nullptr; }

	RawPointerTracker* GetRawPointerTracker() { return &rawPointerTracker; }
protected:
	RawPointerTracker rawPointerTracker;
};