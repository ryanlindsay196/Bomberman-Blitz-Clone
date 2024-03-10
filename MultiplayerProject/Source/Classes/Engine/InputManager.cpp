#include "Engine/InputManager.h"
#include "Game/GameManager.h"

std::vector<Input> inputs;

void InputManager::Initialize()
{
}

void InputManager::Update()
{
	//Update inputs from the previous frame
	for (unsigned int i = 0; i < inputs.size(); ++i)
	{
		inputs[i].isConsumed = false;
		switch (inputs[i].inputState)
		{
		case InputState::Pressed:
			inputs[i].inputState = InputState::Down;
			break;
		case InputState::Released:
			inputs.erase(inputs.begin() + i);
			--i;
			break;
		}
	}

	PollEvents();
}

bool InputManager::WantsToQuit() const
{
	return wantsToQuit;
}

Input& InputManager::GetInputByKeycode(SDL_Keycode keyCode, const InputState desiredInputState) const
{
	for (Input& input : inputs)
	{
		SDL_KeyboardEvent& key = input.e.key;
		if (key.keysym.sym == keyCode && input.inputState == desiredInputState)
		{
			return input;
		}
	}

	return nullInput;
}

inline bool CanConsumeMouseInput(Input& input, InputState inputStateToCheck, Uint8 inButtonId)
{
	return input.e.type == SDL_MOUSEBUTTONDOWN && input.e.button.button == inButtonId && 
		(input.inputState == inputStateToCheck || inputStateToCheck == InputState::AnyState);
}

Input& InputManager::GetInputByMouseButtonID(Uint8 mouseButtonID) const
{
	for (Input& input : inputs)
	{
		if (CanConsumeMouseInput(input, InputState::AnyState, mouseButtonID))
		{
			return input;
		}
	}

	return nullInput;
}

bool InputManager::IsKeyPressed(SDL_Keycode keyCode, bool canConsumeInput) const
{
	if (!GameManager::IsUpdatingFocusedWindow())
	{
		return false;
	}

	Input& input = GetInputByKeycode(keyCode, InputState::Pressed);
	bool isPressed = input.inputState == InputState::Pressed && !input.isConsumed;

	if (isPressed && canConsumeInput)
	{
		input.isConsumed = true;
	}

	return isPressed;
}

bool InputManager::IsKeyReleased(SDL_Keycode keyCode, bool canConsumeInput) const
{
	if (!GameManager::IsUpdatingFocusedWindow())
	{
		return false;
	}

	Input& input = GetInputByKeycode(keyCode, InputState::Released);
	bool isReleased = input.inputState == InputState::Released && !input.isConsumed;

	if (isReleased && canConsumeInput)
	{
		input.isConsumed = true;
	}

	return isReleased;
}

bool InputManager::IsKeyDown(SDL_Keycode keyCode, bool canConsumeInput) const
{
	if (!GameManager::IsUpdatingFocusedWindow())
	{
		return false;
	}

	Input& input = GetInputByKeycode(keyCode, InputState::Down);
	bool isDown = input.inputState == InputState::Down && !input.isConsumed;

	if (isDown && canConsumeInput)
	{
		input.isConsumed = true;
	}

	return isDown;
}

bool InputManager::IsMouseButtonPressed(Uint8 inButtonId, bool consumeEvent) const
{
	if (!GameManager::IsUpdatingFocusedWindow())
	{
		return false;
	}

	Input& input = GetInputByMouseButtonID(inButtonId);
	bool isPressed = input.inputState == InputState::Pressed && !input.isConsumed;
	if (isPressed && CanConsumeMouseInput(input, InputState::Pressed, inButtonId) && consumeEvent)
	{
		input.isConsumed = true;
	}
	return isPressed;
}

bool InputManager::IsMouseButtonReleased(Uint8 inButtonId, bool consumeEvent) const
{
	if (!GameManager::IsUpdatingFocusedWindow())
	{
		return false;
	}

	Input& input = GetInputByMouseButtonID(inButtonId);
	bool isReleased = input.inputState == InputState::Released && !input.isConsumed;
	if (isReleased && CanConsumeMouseInput(input, InputState::Released, inButtonId) && consumeEvent)
	{
		input.isConsumed = true;
	}
	return isReleased;
}

bool InputManager::IsMouseButtonDown(Uint8 inButtonId, bool consumeEvent) const
{
	if (!GameManager::IsUpdatingFocusedWindow())
	{
		return false;
	}

	Input& input = GetInputByMouseButtonID(inButtonId);
	bool isDown = input.inputState == InputState::Down && !input.isConsumed;
	if (isDown && CanConsumeMouseInput(input, InputState::Down, inButtonId) && consumeEvent)
	{
		input.isConsumed = true;
	}
	return isDown;
}

void InputManager::HandleWindowEvent(const SDL_Event & e)
{
	GameManager& gameManager = GameManager::Get();
	Renderer* renderer = gameManager.GetRendererFromWindowID(e.window.windowID);
	
	if (e.window.event == SDL_WINDOWEVENT_CLOSE)
	{
#if InEditor
		gameManager.CloseGameInstances();
		//TODO: This assumes that the only way to get a null renderer is for the window to be the editor window.
		SDL_Window* window = SDL_GetWindowFromID(e.window.windowID);
		SDL_HideWindow(window);
#endif
#if ClientMode
		wantsToQuit = true;
#endif
	}

	if (!renderer)
	{
		return;
	}
	renderer->HandleWindowEvent(e);
}

void InputManager::HandleKeyEvent(const SDL_Event & e)
{
	if (e.key.repeat)
	{
		return;
	}

	switch (e.key.type)
	{
	case SDL_KEYDOWN:
		if (!IsKeyPressed(e.key.keysym.sym, false) && !IsKeyDown(e.key.keysym.sym, false) && !IsKeyReleased(e.key.keysym.sym, false))
		{
			inputs.push_back({ e, InputState::Pressed, false });
		}
		else
		{
			printf("InputManager::Error: %i key is already pressed!\n", e.key.keysym.sym);
		}
		break;
	case SDL_KEYUP:
		for (Input& input : inputs)
		{
			if (input.e.key.keysym.sym == e.key.keysym.sym)
			{
				input.inputState = InputState::Released;
				break;
			}
		}
	}
}

void InputManager::HandleMouseEvent(const SDL_Event& e)
{
	switch (e.button.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		for (Input& input : inputs)
		{
			if (input.e.type == e.type && input.inputState == InputState::Pressed)
			{
				assert(false && "Error: Mouse pressed input is already present inside the inputs array.");
			}
		}
		inputs.push_back({ e, InputState::Pressed, false });
		break;
	case SDL_MOUSEBUTTONUP:
		for (Input& input : inputs)
		{
			if (input.e.type == SDL_MOUSEBUTTONDOWN)
			{
				input.inputState = InputState::Released;
			}
		}
		break;
	}
}

void InputManager::PollEvents()
{
	SDL_Event e;

	//Poll for newly pressed/released inputs
	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
		case SDL_WINDOWEVENT_CLOSE:
			wantsToQuit = true;
			break;
		case SDL_WINDOWEVENT:
			HandleWindowEvent(e);
			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			HandleKeyEvent(e);
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			HandleMouseEvent(e);
			break;
		}
	}
}
