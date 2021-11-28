#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "rpg/engine.h"
#include "rpg/resources/texturemanager.h"
#include <string>
#include <map>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

void TextureManager::Initalize()
{
    std::string path = "assets/sprites";
    for (const auto& entry : fs::recursive_directory_iterator(path))
    {
        // Get the path as a string.
        std::string path = entry.path().string();

        // Replace the slashes in our path with forward slashes.
        int stringIndex;
        while ((stringIndex = path.find("\\")) != std::string::npos)
        {
            path.replace(stringIndex, std::string("/").length(), std::string("/"));
        }

        // We only load PNG files. This check is here as folders count under the iterator
        // and we obviously can't load a folder.
        if (path.ends_with(".png")) LoadTexture(path);
    }
}

SDL_Texture* TextureManager::GetTexture(std::string texturePath)
{
    // Double check to see if this texture exists.
    if (!TextureExists(texturePath)) return NULL;

    // Find our texture with the texture path.
    auto iter = textures.find(texturePath);
    return iter->second;
}

bool TextureManager::TextureExists(std::string texturePath)
{
    // Find our texture with the texture path.
    auto iter = textures.find(texturePath);
    return !(iter == textures.end());
}

bool TextureManager::RemoveTexture(std::string texturePath)
{
    // Double check to see if this texture exists.
    if (!TextureExists(texturePath)) return false;

    // Get the texture from our map of textures.
    SDL_Texture* texture = GetTexture(texturePath);

    // Destroy the texture.
    SDL_DestroyTexture(texture);

    // Remove this texture from the map.
    textures.erase(texturePath);
    return true;
}

void TextureManager::ReleaseAllTextures()
{
    // Loop through all of our textures and release them.
    for (std::map<std::string, SDL_Texture*>::iterator iter = textures.begin();
    iter != textures.end(); iter++)
    {
        RemoveTexture(iter->first);
    }
}

bool TextureManager::LoadTexture(std::string texturePath)
{
    if (EngineResources.renderer == NULL)
    {
        // Stop.
        printf("[ERROR] Could not load texture %s.\n", texturePath.c_str());
        return false;
    }

    // Create a texture from the surface.
    SDL_Texture* texture = IMG_LoadTexture(EngineResources.renderer, texturePath.c_str());
    if (texture == NULL) 
    {
        printf("[ERROR] Could not create texture %s: %s.\n", texturePath.c_str(), IMG_GetError());
        return false;
    }

    // If this texture already exists in our map, remove it first, and then
    // put in this new texture.
    if (TextureExists(texturePath))
    {
        // Remove it.
        RemoveTexture(texturePath);
    }
    // Store this new texture in our map.
    textures.insert(std::pair<std::string, SDL_Texture*>(texturePath, texture));
    printf("[TEXTURES] Texture loaded: %s.\n", texturePath.c_str());
    return true;
}