#include "SDL.h"
#include "SDL_image.h"
#include "Engine/Rendering/Renderer.h"

#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool Renderer::Initialize()
{
	window = nullptr;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	//Create window
	window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window)
	{
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	SDLRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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

	return false;
}

void Renderer::UpdateRender(SDL_Texture * textureToAdd, SDL_Rect * srcRect, SDL_Rect * destRect)
{
	SDL_RenderCopy(SDLRenderer, textureToAdd, srcRect, destRect);
}

void Renderer::Render()
{
	SDL_RenderPresent(SDLRenderer);
	SDL_RenderClear(SDLRenderer);
}

SDL_Texture* Renderer::LoadTexture(const char * path)
{
	SDL_Texture* newTexture = nullptr;
	SDL_Surface* loadedSurface = IMG_Load(path);

	if (!loadedSurface)
	{
		std::cout << "Unable to load image " << path << "! SDL Error: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	newTexture = SDL_CreateTextureFromSurface(SDLRenderer, loadedSurface);

	if (!newTexture)
	{
		std::cout << "Unable to create texture from " << path << " ! SDL Error: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	SDL_FreeSurface(loadedSurface);

	return newTexture;
}

void Renderer::Destroy()
{
	SDL_DestroyRenderer(SDLRenderer);
	SDL_DestroyWindow(window);

	SDLRenderer = nullptr;
	window = nullptr;
}