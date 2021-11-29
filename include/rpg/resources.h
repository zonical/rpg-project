#ifndef HEADER_H_RENDERER
#define HEADER_H_RENDERER

#include <map>
#include <string>
#include <vector>
#include <memory>
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

#include "rpg/entity.h"
#include "rpg/level/level.h"
#include "rpg/resources/texturemanager.h"
#include "rpg/resources/fontmanager.h"
#include "rpg/resources/dialoguemanager.h"
#include "rpg/resources/tilesetmanager.h"

#define DEFAULT_SCREEN_WIDTH    1200
#define DEFAULT_SCREEN_HEIGHT   700

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

    // The main window for our application.
    SDL_Window *window = NULL;

    // The camera that everything revolves around.
    SDL_FRect camera = { 0, 0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT };
public:
    void OnPreRender();
    void RenderEntities(std::vector<Entity*>);
    void RenderLevel(Level* level);
    void RenderMisc();
    void FinishRender();

    bool Initalize();
    void Shutdown();
};
#endif