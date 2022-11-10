#pragma once
#include <unordered_map>
#include <memory>

struct SDL_Texture;
struct SDL_Renderer;
class Renderer;

class Texture
{
	friend class TextureManager;

public:
	~Texture();

	SDL_Texture* GetTexture() { return texture; }

private:
	void LoadFromFilePath(Renderer* renderer, const char* path);
	SDL_Texture* texture;
};

class TextureManager
{
public:
	std::shared_ptr<Texture> LoadTexture(Renderer* renderer, const char* path);

private:
	std::unordered_map<const char*, std::weak_ptr<Texture>> textures;
};