#pragma once
#include <vector>
#include "SDL.h"

enum InputState
{
	NullState,
	//The key was pressed on this frame
	Pressed,
	//The key was pressed on a previous frame
	Down,
	//The key was released this frame
	Released,
	AnyState
};

struct Input
{
	SDL_Event e;
	InputState inputState;
	bool isConsumed;
};

static Input nullInput;

class InputManager
{
public:
	void Initialize();
	void Update();
	bool WantsToQuit() const;

	Input& GetInputByKeycode(SDL_Keycode keyCode, const InputState desiredInputState) const;
	Input& GetInputByMouseButtonID(Uint8 mouseButtonID) const;

	bool IsKeyPressed(SDL_Keycode keyCode, bool canConsumeInput) const;
	bool IsKeyReleased(SDL_Keycode keyCode, bool canConsumeInput) const;
	bool IsKeyDown(SDL_Keycode keyCode, bool canConsumeInput) const;

	bool IsMouseButtonPressed(Uint8 inButtonId, bool consumeEvent) const;
	bool IsMouseButtonReleased(Uint8 inButtonId, bool consumeEvent) const;
	bool IsMouseButtonDown(Uint8 inButtonId, bool consumeEvent) const;

private:
	void HandleWindowEvent(const SDL_Event& e);
	void HandleKeyEvent(const SDL_Event& e);
	void HandleMouseEvent(const SDL_Event& e);

	void PollEvents();

	bool wantsToQuit;
};