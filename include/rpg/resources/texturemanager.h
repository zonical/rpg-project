#pragma once
#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "SDL/SDL.h"
#include <memory>
#include <string>
#include <map>

void DestroyTexturePointer(SDL_Texture* p);

// Responsible for all of our textures.
class TextureManager
{
private:
	// An internal map contianing all of our textures.
	std::map<std::string, std::shared_ptr<SDL_Texture>> textures;
public:
	void Initalize();

	// Releases all textures from memory.
	void ReleaseAllTextures();

	// Loads a texture and stores it internally.
	bool			LoadTexture(std::string texture);   
	// Boolean check to see if a texture exists.
	bool			TextureExists(std::string texture);   
	// Removes a texture and frees it.
	bool			RemoveTexture(std::string texture);
	// Grabs a pointer to a texture.
	std::shared_ptr<SDL_Texture> GetTexture(std::string texture);        
};
#endif