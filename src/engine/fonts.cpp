#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "rpg/renderer.h"
#include <string>
#include <map>

TTF_Font* Renderer::GetFont(std::string fontName, int size)
{
    // Double check to see if this font exists.
    if (!FontExists(fontName, size)) return NULL;

    // Find our font with the font path.
    auto iter = fonts.find(std::make_pair(fontName, size));
    return iter->second;
}

bool Renderer::FontExists(std::string fontName, int size)
{
    // Find our font with the font path and size.
    auto iter = fonts.find(std::make_pair(fontName, size));
    return !(iter == fonts.end());
}

bool Renderer::RemoveFont(std::string fontName, int size)
{
    // Double check to see if this font exists.
    if (!FontExists(fontName, size)) return false;

    // Get the font from our map of fonts.
    TTF_Font* font = GetFont(fontName, size);

    // Destroy the font.
    TTF_CloseFont(font);

    // Remove this font from the map.
    fonts.erase(std::make_pair(fontName, size));
    return true;
}

void Renderer::ReleaseAllFonts()
{
    // Loop through all of our fonts and release them.
    for (std::map<std::pair<std::string, int>, TTF_Font*>::iterator iter = fonts.begin();
        iter != fonts.end(); iter++)
    {
        std::pair <std::string, int> pair = iter->first;
        RemoveFont(pair.first, pair.second);

        delete iter->second;
    }
}

bool Renderer::LoadFont(std::string fontName, std::string fontPath, int size)
{
    if (renderer == NULL)
    {
        // Stop.
        printf("[ERROR] Could not load font %s.\n", fontPath.c_str());
        return false;
    }

    // Create a font from the surface.
    TTF_Font* font = TTF_OpenFont(fontPath.c_str(), size);
    if (font == NULL)
    {
        printf("[ERROR] Could not create font %s: %s.\n", fontName.c_str(), IMG_GetError());
        return false;
    }

    // If this font already exists in our map, remove it first, and then
    // put in this new font.
    if (FontExists(fontName, size))
    {
        // Remove it.
        RemoveFont(fontName, size);
    }

    // Store this new font in our map.
    std::pair <std::string, int> pair = std::make_pair(fontName, size);
    fonts.insert(std::make_pair(pair, font));

    printf("[FONT] Font loaded: %s : %d.\n", fontName.c_str(), size);
    return true;
}