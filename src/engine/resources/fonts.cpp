#include "rpg/rpg.h"

void DestroyFontPointer(TTF_Font* p)
{
    //TTF_CloseFont(p);
}

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
        std::cout << "[FONTS] Failed to initalize FontManager: " << Exception.what() << std::endl;
    }
}

std::shared_ptr<TTF_Font> FontManager::GetFont(std::string fontName, int size)
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

    // Reset our font.
    GetFont(fontName, size).reset();

    // Remove this font from the map.
    fonts.erase(std::make_pair(fontName, size));
    return true;
}

void FontManager::ReleaseAllFonts()
{
    fonts.clear();
}

bool FontManager::LoadFont(std::string fontName, std::string fontPath, int size)
{
    if (EngineResources.renderer == NULL)
    {
        // Stop.
        std::cout << "[ERROR] Could not load font " << fontPath.c_str() << std::endl;
        return false;
    }

    // Create a font.
    std::shared_ptr<TTF_Font> ptr(TTF_OpenFont(fontPath.c_str(), size), &DestroyFontPointer);

    if (ptr.get() == NULL)
    {
        std::cout << "[ERROR] Could not create font " << fontName.c_str() << ": " << IMG_GetError() << std::endl;
        return false;
    }

    // If this font already exists, assigning a new pointer to a shared_ptr will reset it for us.
    if (FontExists(fontName, size))
    {
        // Grab our pointer and change the value of it.
        auto oldPtr = GetFont(fontName, size);
        oldPtr.swap(ptr);
        return true;
    }

    // Store this new font in our map.
    std::pair <std::string, int> pair = std::make_pair(fontName, size);
    fonts.insert(std::make_pair(pair, ptr));

    std::cout << "[FONT] Font loaded: " << fontName.c_str() << ", " << size << std::endl;
    return true;
}