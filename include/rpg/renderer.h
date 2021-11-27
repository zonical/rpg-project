#ifndef HEADER_H_RENDERER
#define HEADER_H_RENDERER

#include <map>
#include <string>
#include <vector>
#include <memory>
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "rpg/entity.h"

#define DEFAULT_SCREEN_WIDTH    1200
#define DEFAULT_SCREEN_HEIGHT   700

class Renderer
{
private:
    // TEXTURES:

    // Main map that contains all of our textures.
    std::map<std::string, SDL_Texture*> textures;
    void ReleaseAllTextures();

    // FONTS
    
    // Main map that contains all of our fonts.
    std::map<std::pair<std::string, int>, TTF_Font*> fonts;
    void ReleaseAllFonts();

public:
    bool            isInitalized = false;

    // Textures.
    bool            LoadTexture(std::string texture);       // Loads a texture and stores it internally.
    bool            TextureExists(std::string texture);     // Boolean check to see if a texture exists.
    bool            RemoveTexture(std::string texture);     // Removes a texture and frees it.
    SDL_Texture*    GetTexture(std::string texture);        // Grabs a pointer to a texture.
 
    // Fonts.
    bool            LoadFont(std::string fontName, std::string fontPath, int size);     /* Loads a font by reference name,
                                                                                        file path and size and stores it internally.*/
    bool            FontExists(std::string fontName, int size);                         // Boolean check to see if a font with $x size exists.
    bool            RemoveFont(std::string fontName, int size);                         // Removes a font and frees it.
    TTF_Font*       GetFont(std::string fontName, int size);                            // Grabs a pointer to a font.

    // The main renderer for our application.
    SDL_Renderer* renderer = NULL;

    // The main window for our application.
    SDL_Window *window = NULL;

    // The camera that everything revolves around.
    SDL_FRect camera = { 0, 0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT };
public:
    void OnPreRender();
    void RenderEntities(std::vector<Entity*>);
    void RenderMisc();
    void FinishRender();

    bool Initalize();
    void Shutdown();
};
#endif