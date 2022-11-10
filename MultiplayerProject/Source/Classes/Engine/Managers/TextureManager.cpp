#include "Engine/Managers/TextureManager.h"

#include "SDL.h"
#include "SDL_image.h"
#include "Game/GameManager.h"

#include <iostream>

std::shared_ptr<Texture> TextureManager::LoadTexture(Renderer* renderer, const char * path)
{
	auto it = textures.find(path);
	if (it == textures.end() || it->second.expired())
	{
		std::shared_ptr<Texture> newTexturePtr = std::make_shared<Texture>();
		newTexturePtr->LoadFromFilePath(renderer, path);
		textures.insert(std::pair<const char*, std::weak_ptr<Texture>>(path, newTexturePtr));
		return newTexturePtr;
	}

	auto sharedTexturePtr = it->second.lock();
	return sharedTexturePtr;
}

Texture::~Texture()
{
	SDL_DestroyTexture(texture);
}

void Texture::LoadFromFilePath(Renderer* renderer, const char * path)
{
	SDL_Texture* newTexture = nullptr;
	SDL_Surface* loadedSurface = IMG_Load(path);
	
	if (!loadedSurface)
	{
		std::cout << "Unable to load image " << path << "! SDL Error: " << SDL_GetError() << std::endl;
		return;
	}

	if (!renderer)
	{
		std::cout << "Unable to find SDLRenderer" << std::endl;
		return;
	}

	newTexture = SDL_CreateTextureFromSurface(renderer->GetSDLRenderer(), loadedSurface);
	
	if (!newTexture)
	{
		std::cout << "Unable to create texture from " << path << " ! SDL Error: " << SDL_GetError() << std::endl;
		return;
	}
	
	SDL_FreeSurface(loadedSurface);
	
	texture = newTexture;
}
