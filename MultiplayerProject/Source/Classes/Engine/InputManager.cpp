#include "Engine/InputManager.h"

void InputManager::Initialize()
{
}

void InputManager::Update()
{
	//Update inputs from the previous frame
	for (int i = 0; i < inputs.size(); ++i)
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

	//Poll for newly pressed/released inputs
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			wantsToQuit = true;
		}
		
		if (e.key.repeat)
		{
			continue;
		}

		switch(e.key.type)
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
}

bool InputManager::WantsToQuit() const
{
	return wantsToQuit;
}

bool InputManager::IsKeyPressed(SDL_Keycode keyCode, bool consumeEvent)
{
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

bool InputManager::IsKeyReleased(SDL_Keycode keyCode, bool consumeEvent)
{
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

bool InputManager::IsKeyDown(SDL_Keycode keyCode, bool consumeEvent)
{
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


