#include "Engine/InputManager.h"
#include "Game/GameManager.h"

std::vector<Input> inputs;

bool IsUpdatingFocusedWindow()
{
	static GameManager* gameManager = &GameManager::Get();

	Uint32 currentWindowFlags = gameManager->GetRenderer()->GetWindowFlags();
	bool isUpdatingFocusedWindow = currentWindowFlags & SDL_WINDOW_INPUT_FOCUS;
	return isUpdatingFocusedWindow;
}


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

bool InputManager::IsKeyPressed(SDL_Keycode keyCode, bool consumeEvent) const
{
	if (!IsUpdatingFocusedWindow())
	{
		return false;
	}

	for (Input& input : inputs)
	{
		SDL_KeyboardEvent& key = input.e.key;
		if (key.keysym.sym == keyCode && input.inputState == InputState::Pressed)
		{
			if (input.isConsumed)
			{
				return false;
			}
	
			if (consumeEvent)
			{
				input.isConsumed = true;
			}
			return true;
		}
	}
	return false;
}

bool InputManager::IsKeyReleased(SDL_Keycode keyCode, bool consumeEvent) const
{
	if (!IsUpdatingFocusedWindow())
	{
		return false;
	}

	for (Input& input : inputs)
	{
		SDL_KeyboardEvent& key = input.e.key;
		if (key.keysym.sym == keyCode && input.inputState == InputState::Released)
		{
			if (input.isConsumed)
			{
				return false;
			}
	
			if (consumeEvent)
			{
				input.isConsumed = true;
			}
			return true;
		}
	}
	return false;
}

bool InputManager::IsKeyDown(SDL_Keycode keyCode, bool consumeEvent) const
{
	if (!IsUpdatingFocusedWindow())
	{
		return false;
	}

	for (Input& input : inputs)
	{
		SDL_KeyboardEvent& key = input.e.key;
		if (key.keysym.sym == keyCode && input.inputState == InputState::Down)
		{
			if (input.isConsumed)
			{
				return false;
			}
	
			if (consumeEvent)
			{
				input.isConsumed = true;
			}
			return true;
		}
	}
	return false;
}


void InputManager::HandleWindowEvent(const SDL_Event & e)
{
	GameManager& gameManager = GameManager::Get();
	Renderer* renderer = gameManager.GetRendererFromWindowID(e.window.windowID);
	
	if (e.window.event == SDL_WINDOWEVENT_CLOSE)
	{
#if RunInEngine
		gameManager.CloseGameInstances();
		//TODO: This assumes that the only way to get a null renderer is for the window to be the editor window.
		SDL_Window* window = SDL_GetWindowFromID(e.window.windowID);
		SDL_HideWindow(window);
#endif
#if RunClientMode
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

void InputManager::PollEvents()
{
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
		}
	}
}
