#pragma once

struct SDL_Surface;
struct SDL_Rect;
struct SDL_Renderer;
union SDL_Event;

class Texture;

class Renderer
{
public:
	bool Initialize(unsigned int inWidth, unsigned int inHeight, bool shouldMaximizeWindow);

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
	struct SDL_Window* window;

	int windowWidth, windowHeight;
	int viewportWidth, viewportHeight;

	//Window focus
	bool hasMouseFocus;
	bool hasKeyboardFocus;
	bool isFullScreen;
	bool isMinimized;
};