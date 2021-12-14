#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "rpg/engine.h"
#include "rpg/resources/texturemanager.h"
#include <string>
#include <map>
#include <iostream>
#include <filesystem>
#include <memory>
#include <functional>

namespace fs = std::filesystem;

void DestroyTexturePointer(SDL_Texture* p)
{
    SDL_DestroyTexture(p);
}


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

std::shared_ptr<SDL_Texture> TextureManager::GetTexture(std::string texturePath)
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
    std::shared_ptr<SDL_Texture> texture = GetTexture(texturePath);

    // Destroy the texture.
    texture.reset();

    // Remove this texture from the map.
    textures.erase(texturePath);
    return true;
}

void TextureManager::ReleaseAllTextures()
{
    // Loop through all of our textures and release them.
    textures.clear();
}

bool TextureManager::LoadTexture(std::string texturePath)
{
    if (EngineResources.renderer == NULL)
    {
        // Stop.
        std::cout << "[ERROR] Could not load texture " <<  texturePath.c_str() << std::endl;
        return false;
    }

    std::shared_ptr<SDL_Texture> ptr(IMG_LoadTexture(EngineResources.renderer, texturePath.c_str()), &DestroyTexturePointer);

    // If this texture already exists, we can swap out the pointer.
    if (TextureExists(texturePath))
    {
        // Grab our pointer and change the value of it.
        auto oldPtr = GetTexture(texturePath);
        oldPtr.swap(ptr);
        return true;
    }

    // Store this new texture in our map.
    textures.insert(std::pair<std::string, std::shared_ptr<SDL_Texture>>(texturePath, ptr));
    std::cout << "[TEXTURES] Texture loaded: " << texturePath.c_str() << std::endl;
    return true;
}