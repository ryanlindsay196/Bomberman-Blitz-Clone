#pragma once
#include <unordered_map>
#include <memory>

class SDL_Texture;

class Texture
{
	friend class TextureManager;

public:
	~Texture();

	SDL_Texture* GetTexture() { return texture; }

private:
	void LoadFromFilePath(const char* path);
	SDL_Texture* texture;
};

class TextureManager
{
public:
	std::shared_ptr<Texture> LoadTexture(const char* path);

private:
	std::unordered_map<const char*, std::weak_ptr<Texture>> textures;
};