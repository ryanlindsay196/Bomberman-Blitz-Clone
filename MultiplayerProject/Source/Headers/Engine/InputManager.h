#pragma once
#include <vector>
#include "SDL.h"

enum InputState
{
	//The key was pressed on this frame
	Pressed,
	//The key was pressed on a previous frame
	Down,
	//The key was released this frame
	Released
};

struct Input
{
	SDL_Event e;
	InputState inputState;
	bool isConsumed;
};

class InputManager
{
public:
	void Initialize();
	void Update();
	bool WantsToQuit() const;

	bool IsKeyPressed(SDL_Keycode keyCode, bool consumeEvent);
	bool IsKeyReleased(SDL_Keycode keyCode, bool consumeEvent);
	bool IsKeyDown(SDL_Keycode keyCode, bool consumeEvent);

private:
	SDL_Event e;
	bool wantsToQuit;
};