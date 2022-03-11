#ifndef HEADER_H_RENDERER
#define HEADER_H_RENDERER
#include <map>
#include <string>
#include <vector>
#include <memory>
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

#include "rpg/resources/texturemanager.h"
#include "rpg/resources/fontmanager.h"
#include "rpg/resources/dialoguemanager.h"
#include "rpg/resources/tilesetmanager.h"
#include "rpg/gamestate.h"

#define DEFAULT_SCREEN_WIDTH    1200
#define DEFAULT_SCREEN_HEIGHT   700

#define FADE_FROMBLACK 0
#define FADE_TOBLACK 1
#define FADE_HOLD 2

class Resources
{
public:
    // Holds all of our textures.
    TextureManager textures;

    // Holds all of our fonts.
    FontManager fonts;

    // Holds all of our dialogue.
    DialogueManager dialogue;

    // Holds all of our tilesets.
    TilesetManager tilesets;

    // The main renderer for our application.
    SDL_Renderer* renderer = NULL;

    // The background color we default to when rendering.
    SDL_Color backgroundColor;

    // The main window for our application.
    SDL_Window *window = NULL;
public:
    // Rendering functions.
    void OnPreRender();
    void RenderState(BaseGameState* state);
    void RenderMisc();
    void FinishRender();

    // Construction and deconstruction functions.
    bool Initalize();
    void Shutdown();
 
    // Fading functions.
    void FadeFromBlack(float duration);
    void FadeToBlack(float duration);
    void ResetFade();
    void ApplyGlobalFade(SDL_Color color);
    bool currentlyFading;

private:
    SDL_Rect fadeRect = { 0, 0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT };
    SDL_Color fadeColor;
    float fadeDuration;
    float fadeDelta;
    float fadeProgress;
    int fadeType = -1;
};

// Python module init function. This is required in every header file for classes that
// might expose C++ functions to Python!
extern "C" BOOST_SYMBOL_EXPORT PyObject *PyInit_resources();
#endif