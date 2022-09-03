#pragma once

class SDL_Surface;
class SDL_Rect;
class SDL_Texture;
class SDL_Renderer;

class Renderer
{
public:
	bool Initialize();

	void UpdateRender(SDL_Texture* textureToAdd, SDL_Rect* srcRect, SDL_Rect* destRect);
	void Render();

	SDL_Texture* LoadTexture(const char* path);

	void Destroy();

private:
	SDL_Renderer* SDLRenderer;
	class SDL_Window* window;
};