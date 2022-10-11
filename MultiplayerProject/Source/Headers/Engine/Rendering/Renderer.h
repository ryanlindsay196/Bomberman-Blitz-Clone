#pragma once

class SDL_Surface;
class SDL_Rect;
class SDL_Texture;
class SDL_Renderer;
union SDL_Event;

class Renderer
{
public:
	bool Initialize();

	void UpdateRender(SDL_Texture* textureToAdd, SDL_Rect* srcRect, SDL_Rect* destRect);
	void Render();

	SDL_Texture* LoadTexture(const char* path);

	void Destroy();

	int GetWidth() { return windowWidth; }
	int GetHeight() { return windowHeight; }

	bool HasMouseFocus() { return hasMouseFocus; }
	bool HasKeyboardFocus() { return hasKeyboardFocus; }
	bool IsMinimized() { return isMinimized; }

	void HandleWindowEvent(SDL_Event& e);

private:
	SDL_Renderer* SDLRenderer;
	class SDL_Window* window;

	int windowWidth, windowHeight;

	//Window focus
	bool hasMouseFocus;
	bool hasKeyboardFocus;
	bool isFullScreen;
	bool isMinimized;
};