#include "SDL.h"
#include "SDL_image.h"
#include "Engine/Rendering/Renderer.h"
#include "Engine/Managers/TextureManager.h"

#include <iostream>

constexpr int DEFAULT_SCREEN_WIDTH = 640;
constexpr int DEFAULT_SCREEN_HEIGHT = 480;
constexpr float DEFAULT_ASPECT_RATIO = (float)DEFAULT_SCREEN_WIDTH / DEFAULT_SCREEN_HEIGHT;

bool Renderer::Initialize(unsigned int inWidth, unsigned int inHeight, bool shouldMaximizeWindow, const char* windowName)
{
	window = nullptr;

	hasMouseFocus = true;
	hasKeyboardFocus = true;
	isFullScreen = false;
	isMinimized = false;
	windowWidth = inWidth;
	windowHeight = inHeight;
	viewportWidth = DEFAULT_SCREEN_WIDTH;
	viewportHeight = DEFAULT_SCREEN_HEIGHT;
	TryUpdateViewportSize();

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	//Create window
	window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (!window)
	{
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	SDLRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!SDLRenderer)
	{
		std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
		return false;
	}

	SDL_SetRenderDrawColor(SDLRenderer, 0x00, 0x00, 0x00, 0xFF);

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
		return false;
	}

	SDL_GetWindowSurface(window);
	//Update the surface
	SDL_UpdateWindowSurface(window);

	if (shouldMaximizeWindow)
	{
		SDL_MaximizeWindow(window);
	}

	return false;
}

void Renderer::Update(float deltaTime)
{
	TryUpdateViewportSize();
	SDL_RenderSetLogicalSize(SDLRenderer, viewportWidth, viewportHeight);
}

void Renderer::UpdateRender(Texture* textureToAdd, SDL_Rect srcRect, SDL_Rect destRect, SDL_Color color)
{
	SDL_SetTextureColorMod(textureToAdd->GetTexture(), color.r, color.g, color.b);

	destRect.x = destRect.x * (int)((float)viewportWidth / DEFAULT_SCREEN_WIDTH);
	destRect.w = destRect.w * (int)((float)viewportWidth / DEFAULT_SCREEN_WIDTH);
	
	destRect.y = destRect.y * (int)((float)viewportHeight / DEFAULT_SCREEN_HEIGHT);
	destRect.h = destRect.h * (int)((float)viewportHeight / DEFAULT_SCREEN_HEIGHT);

	SDL_Texture* sdlTexToAdd = textureToAdd->GetTexture();

	SDL_RenderCopy(SDLRenderer, sdlTexToAdd, &srcRect, &destRect);
}

void Renderer::Render()
{
	if (!isMinimized)
	{
		SDL_RenderPresent(SDLRenderer);
		SDL_RenderClear(SDLRenderer);
	}
}

void Renderer::Destroy()
{
	SDL_DestroyRenderer(SDLRenderer);
	SDL_DestroyWindow(window);

	SDLRenderer = nullptr;
	window = nullptr;
}

float Renderer::GetWindowAspectRatio() const
{
	return (float)windowWidth / (float)windowHeight;
}

float Renderer::GetViewportAspectRatio() const
{
	return (float)viewportWidth / (float)viewportHeight;
}

void Renderer::TryUpdateViewportSize()
{
	int oldWindowWidth = windowWidth;
	int oldWindowHeight = windowHeight;
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);

	bool viewportSizeChanged = oldWindowWidth != windowWidth || oldWindowHeight != windowHeight;
	if (!viewportSizeChanged)
	{
		return;
	}

	if (GetWindowAspectRatio() > DEFAULT_ASPECT_RATIO)
	{
		viewportHeight = windowHeight;
		viewportWidth = (int)(DEFAULT_ASPECT_RATIO * windowHeight);
	}
	else
	{
		viewportWidth = windowWidth;
		viewportHeight = (int)(windowWidth / DEFAULT_ASPECT_RATIO);
	}
}

float Renderer::GetDefaultAspectRatio() const
{
	return DEFAULT_ASPECT_RATIO;
}

void Renderer::HandleWindowEvent(const SDL_Event& e)
{
	switch (e.window.event)
	{
		//Get new dimensions and repaint on window size change
	case SDL_WINDOWEVENT_SIZE_CHANGED:
		windowWidth = e.window.data1;
		windowHeight = e.window.data2;

		TryUpdateViewportSize();
		SDL_RenderPresent(SDLRenderer);
		break;

		//Repaint on exposure
	case SDL_WINDOWEVENT_EXPOSED:
		SDL_RenderPresent(SDLRenderer);
		break;
	
		//Mouse entered window
	case SDL_WINDOWEVENT_ENTER:
		hasMouseFocus = true;
		break;
	
		//Mouse left window
	case SDL_WINDOWEVENT_LEAVE:
		hasMouseFocus = false;
		break;
	
		//Window has keyboard focus
	case SDL_WINDOWEVENT_FOCUS_GAINED:
		hasKeyboardFocus = true;
		break;
	
		//Window lost keyboard focus
	case SDL_WINDOWEVENT_FOCUS_LOST:
		hasKeyboardFocus = false;
		break;
	
		//Window minimized
	case SDL_WINDOWEVENT_MINIMIZED:
		isMinimized = true;
		break;
	
		//Window maximized
	case SDL_WINDOWEVENT_MAXIMIZED:
		isMinimized = false;
		break;
	
		//Window restored
	case SDL_WINDOWEVENT_RESTORED:
		isMinimized = false;
		break;
	}
	
	//Enter exit full screen on return key
	if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
	{
		if (isFullScreen)
		{
			SDL_SetWindowFullscreen(window, SDL_FALSE);
			isFullScreen = false;
		}
		else
		{
			SDL_SetWindowFullscreen(window, SDL_TRUE);
			isFullScreen = true;
			isMinimized = false;
		}
	}
}

void Renderer::Close()
{
	SDL_HideWindow(window);
}

void Renderer::Open()
{
	SDL_ShowWindow(window);
}

bool Renderer::IsOpen()
{ 
	return SDL_GetWindowFlags(window) & SDL_WINDOW_SHOWN;
}

Uint32 Renderer::GetWindowID() const
{
	return SDL_GetWindowID(window);
}

unsigned int Renderer::GetWindowFlags() const
{
	return SDL_GetWindowFlags(window);
}
