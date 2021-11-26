#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "rpg/renderer.h"
#include <string>
#include <map>

SDL_Texture* Renderer::GetTexture(std::string texturePath)
{
    // Double check to see if this texture exists.
    if (!TextureExists(texturePath)) return NULL;

    // Find our texture with the texture path.
    auto iter = textures.find(texturePath);
    return iter->second;
}

bool Renderer::TextureExists(std::string texturePath)
{
    // Find our texture with the texture path.
    auto iter = textures.find(texturePath);
    return !(iter == textures.end());
}

bool Renderer::RemoveTexture(std::string texturePath)
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

void Renderer::ReleaseAllTextures()
{
    // Loop through all of our textures and release them.
    for (std::map<std::string, SDL_Texture*>::iterator iter = textures.begin();
    iter != textures.end(); iter++)
    {
        RemoveTexture(iter->first);
    }
}

bool Renderer::LoadTexture(std::string texturePath)
{
    if (renderer == NULL)
    {
        // Stop.
        printf("[ERROR] Could not load texture %s.\n", texturePath.c_str());
        return false;
    }

    // Create a texture from the surface.
    SDL_Texture* texture = IMG_LoadTexture(renderer, texturePath.c_str());
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