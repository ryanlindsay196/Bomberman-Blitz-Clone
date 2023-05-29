#pragma once
#include "Engine/Rendering/Renderer.h"
#include "Engine/UI/UIManager.h"

class Engine
{
public:
	void Initialize();
	bool Update(float deltaTime);

	Renderer* GetRenderer() { return &renderer; }
private:
	void PlayInEditor();

	Renderer renderer;
	UIManager uiManager;
};