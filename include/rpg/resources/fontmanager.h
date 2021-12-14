#pragma once
#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

#include <string>
#include <map>
#include <memory>

void DestroyFontPointer(TTF_Font* p);

// Responsible for all of our fonts.
class FontManager
{
private:
    // Main map that contains all of our fonts.
    std::map<std::pair<std::string, int>, std::shared_ptr<TTF_Font>> fonts;
public:
    void Initalize();

    // Releases all fonts from memory.
    void ReleaseAllFonts();

    // Loads a font by reference name, file pathand sizeand stores it internally.
    bool        LoadFont(std::string fontName, std::string fontPath, int size);
    // Boolean check to see if a font with $x size exists.
    bool        FontExists(std::string fontName, int size);
    // Removes a font and frees it.
    bool        RemoveFont(std::string fontName, int size);
    // Grabs a pointer to a font.
    std::shared_ptr<TTF_Font> GetFont(std::string fontName, int size);
};

#endif // !FONTMANAGER_H
