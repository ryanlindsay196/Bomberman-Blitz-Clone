#pragma once

class SDL_Surface;
class SDL_Rect;
class SDL_Renderer;
union SDL_Event;

class Texture;

class Renderer
{
public:
	bool Initialize();

	void Update(float deltaTime);

	void UpdateRender(Texture* textureToAdd, SDL_Rect* srcRect, SDL_Rect* destRect);
	void Render();

	void Destroy();

	int GetWidth() { return windowWidth; }
	int GetHeight() { return windowHeight; }

	bool HasMouseFocus() { return hasMouseFocus; }
	bool HasKeyboardFocus() { return hasKeyboardFocus; }
	bool IsMinimized() { return isMinimized; }

	void HandleWindowEvent(SDL_Event& e);

	SDL_Renderer* GetSDLRenderer() { return SDLRenderer; }

private:
	float GetWindowAspectRatio();
	float GetViewportAspectRatio();
	void CalculateViewportSize();

	SDL_Renderer* SDLRenderer;
	class SDL_Window* window;

	int windowWidth, windowHeight;
	int viewportWidth, viewportHeight;

	//Window focus
	bool hasMouseFocus;
	bool hasKeyboardFocus;
	bool isFullScreen;
	bool isMinimized;
};