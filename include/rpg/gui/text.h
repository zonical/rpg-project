#ifndef TEXT_H
#define TEXT_H

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "rpg/gui/base.h"
#include "rpg/engine.h"
#include <memory>
#include <string>

#define DEFAULT_FONT        "40573_VIDEOTER"
#define DEFAULT_FONT_SIZE   32

// Base class for a textbox in-game.
class Text : public GUIElement
{
private:
    // Pointers for resources.
    std::shared_ptr<TTF_Font> font = NULL;
    SDL_Texture*    texture = NULL;

    // Current displayed text.
    std::string     text;

    // Internally storing the size of the font.
    int fontSize;
    int             wrappingWidth;

public:
    bool isInitalized = false;

    Text();
    Text(int layer, std::string elementName);

    // If this text is static and will NEVER be updated, we will not create a new surface and texture
    // every single frame to render text.
    bool isTextStatic = true;

    ~Text()
    {
        this->font.reset();
        //SDL_DestroyTexture(texture);
    };

    void Draw(SDL_Window*, SDL_Renderer*);
    void SetText(std::string newText) 
    { 
        text = newText; 
        if (isInitalized) CreateTextTexture();
    };
    void SetTextColor(SDL_Color newColor)
    {
        this->colorModifier = newColor;
        if (isInitalized) CreateTextTexture();
    };
    void SetFont(std::shared_ptr<TTF_Font> newFont)
    { 
        font = newFont;
        if (isInitalized) CreateTextTexture();
    };

    // Creates a texture and stores it internally.
    bool CreateTextTexture();

    // Create our inital TextTexture on spawn.
    void OnElementSpawned();
};
#endif // !TEXT_H