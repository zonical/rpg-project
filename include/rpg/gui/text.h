#ifndef TEXT_H
#define TEXT_H
#include <string>
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "rpg/gui/base.h"
#include "rpg/engine.h"

#define DEFAULT_FONT        "40573_VIDEOTER"
#define DEFAULT_FONT_SIZE   32

// A struct to define our text settings since we'll have so many parameters for
// our constructor.
struct TextSettings
{
    float           x = 0.0f;
    float           y = 0.0f;
    float           fixedWidth = 0.0f;
    float           fixedHeight = 0.0f;
    int             wrappingWidth = 0.0f;
    std::string     text = "";
    SDL_Color       color = {255, 255, 255, 255};
};

// Base class for a textbox in-game.
class Text : public GUIElement
{
private:
    TTF_Font*       font = NULL;
    SDL_Texture*    texture = NULL;

    std::string     text;
    SDL_Color       textColor;

    int             wrappingWidth;
    bool            customHeight;
    bool            customWidth;
public:
    Text();
    Text(int layer, TextSettings settings);

    void Init();

    // If this text is static and will NEVER be updated, we will not create a new surface and texture
    // every single frame to render text.
    bool isTextStatic = true;

    ~Text()
    {
        GameEngine->gGUI.RemoveElement(this, guiLayer);
        delete texture;
        delete font;
    };

    void Draw(SDL_Window*, SDL_Renderer*);
    void SetText(std::string newText) 
    { 
        text = newText; 
        CreateTextTexture();
    };
    void SetTextColor(SDL_Color newColor)
    {
        textColor = newColor;
        CreateTextTexture();
    };
    void SetFont(TTF_Font* newFont)
    { 
        font = newFont;
        CreateTextTexture();
    };

    // Creates a texture and stores it internally.
    bool CreateTextTexture();
};
#endif // !TEXT_H