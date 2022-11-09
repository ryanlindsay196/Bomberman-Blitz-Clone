#pragma once

#include "Engine/Rendering/Renderer.h"
#include "Engine/InputManager.h"
#include "Engine/UI/UIManager.h"

class Engine
{
public:
	void Initialize();
	bool Update(float deltaTime);

private:
	Renderer renderer;
	InputManager inputManager;
	UIManager uiManager;
};