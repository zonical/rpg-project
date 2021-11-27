#include "rpg/renderer.h"
#include "rpg/entity.h"
#include "rpg/engine.h"
#include <vector>

bool Renderer::Initalize()
{
    // Create the main window for our application.
    window = SDL_CreateWindow( "RPG Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
    DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

    // Is our window valid?
    if ( window == NULL )
    {
        printf( "[ERROR] Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        return false;
    }

    // Create the renderer for our window.
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Is our renderer valid?
    if( renderer == NULL )
    {
        printf( "[ERROR] Renderer could not be created! SDL_Error: %s\n", SDL_GetError() );
        return false;
    }

    // Init PNG loading:
    if( !( IMG_Init( IMG_INIT_PNG ) ) )
    {
        printf( "[ERROR] SDL_image could not initialize! SDL_image Error: %s\n", SDL_GetError() );
        return false;
    }

    printf("[ENGINE] Renderer initalized.\n");
    isInitalized = true;
    return true;
}

// Release and destroy all of our resources.
void Renderer::Shutdown()
{
    // Destroy window
    SDL_DestroyWindow(window);

    // Destroy renderer.
    SDL_DestroyRenderer(renderer);

    // Release all our textures from memory.
    ReleaseAllTextures();

    // Release all our fonts from memory.
    ReleaseAllFonts();
}

// Steps we need to do before we start rendering this frame.
void Renderer::OnPreRender()
{
    // Clear the screen, set it to black.
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderClear( renderer );
}

// Take all of our entities and call their render functions with
// a reference to our window and renderer if they need it.
void Renderer::RenderEntities(std::vector<Entity*> entities)
{
    for (auto& entity : entities)
    {
        // Call the entities render function.
        if (entity->HasTag(Tag_Renderable) || !entity->HasTag(Tag_NotRendering))
        {
            // Manipulate the destinationRect of this entity to be offset by the camera.
            entity->destinationRect.x = entity->levelX - camera.x;
            entity->destinationRect.y = entity->levelY - camera.y;
            entity->Draw(window, renderer);
        }
    }
}

// Render any other misc things in this function here.
void Renderer::RenderMisc()
{
    // Render the GUI.
    for (auto& layer : GameEngine->gGUI.elements)
    {
        // Call the entities render function.
        for (auto& element : layer)
        {
            if (element->HasTag(Tag_Renderable) || !element->HasTag(Tag_NotRendering))
            {
                element->Draw(window, renderer);
            }
            
        }
    }
}

// Finish rendering to the screen by actually renderign.
void Renderer::FinishRender()
{
    SDL_RenderPresent( renderer );
}