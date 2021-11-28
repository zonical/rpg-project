#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "json/json.hpp"

#include "rpg/engine.h"
#include "rpg/resources/fontmanager.h"
#include <string>
#include <map>

void FontManager::Initalize()
{
    using json = nlohmann::json;

    try
    {
        // Load the file and have it automatically be parsed by the engine.
        auto fontsFile = GameEngine->LoadJSON("assets/scripts/font_manifest.json");

        // Iterate over each element.
        for (auto& el : fontsFile.items())
        {
            // Grab the key name.
            auto& key = el.key();
            auto fontDefinition = el.value().get<json>();

            // Our font definition is another JSON object. It contains a few primary things:
            // The path to the file of the font from "assets/fonts..." and including the extension
            // ".ttf". It also contains an array of sizes since we can't dynamically size these fonts
            // without creating textures for every single size.
            auto fontFilePath = fontDefinition["file"].get<std::string>();
            auto fontSizes = fontDefinition["sizes"].get<std::vector<int>>();   // An array of integers.

            // For each of our sizes, create a new font and store it internally.
            for (auto& size : fontSizes)
            {
                // Load the font from the file.
                LoadFont(key, fontFilePath, size);
            }
        }
    }
    catch (std::exception& Exception)
    {
        printf("[FONTS] Failed to initalize FontManager: %s\n", Exception.what());
    }
}

TTF_Font* FontManager::GetFont(std::string fontName, int size)
{
    // Double check to see if this font exists.
    if (!FontExists(fontName, size)) return NULL;

    // Find our font with the font path.
    auto iter = fonts.find(std::make_pair(fontName, size));
    return iter->second;
}

bool FontManager::FontExists(std::string fontName, int size)
{
    // Find our font with the font path and size.
    auto iter = fonts.find(std::make_pair(fontName, size));
    return !(iter == fonts.end());
}

bool FontManager::RemoveFont(std::string fontName, int size)
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

void FontManager::ReleaseAllFonts()
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

bool FontManager::LoadFont(std::string fontName, std::string fontPath, int size)
{
    if (EngineResources.renderer == NULL)
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