#include "rpg/resources.h"
#include "rpg/entity.h"
#include "rpg/engine.h"
#include <vector>

bool Resources::Initalize()
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
    return true;
}

// Release and destroy all of our resources.
void Resources::Shutdown()
{

    fonts.ReleaseAllFonts();
    textures.ReleaseAllTextures();
    tilesets.ReleaseAllTilesets();

    // Destroy window
    SDL_DestroyWindow(window);

    // Destroy renderer.
    SDL_DestroyRenderer(renderer);
  
}

// Steps we need to do before we start rendering this frame.
void Resources::OnPreRender()
{
    // Clear the screen, set it to black.
    SDL_SetRenderDrawColor( renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a );
    SDL_RenderClear( renderer );
}

bool CollisionCheckF(SDL_FRect a, SDL_FRect b)
{
    // %s1 = us
    // %s2 = other
    float left1, left2;
    float right1, right2;
    float top1, top2;
    float bottom1, bottom2;

    left1 = a.x;
    right1 = a.x + a.w;
    top1 = a.y;
    bottom1 = a.y + a.h;

    left2 = b.x;
    right2 = b.x + b.w;
    top2 = b.y;
    bottom2 = b.y + b.h;

    if (bottom1 <= top2) return false;
    if (top1 >= bottom2) return false;
    if (right1 <= left2) return false;
    if (left1 >= right2) return false;

    // We're not colliding with anything.
    return true;
}

// Take all of our entities and call their render functions with
// a reference to our window and renderer if they need it.
void Resources::RenderEntities(std::vector<Entity*> entities)
{
    for (auto& entity : entities)
    {
        // Manipulate the destinationRect of this entity to be offset by the camera.
        entity->destinationRect.x = entity->levelX - camera.x;
        entity->destinationRect.y = entity->levelY - camera.y;

        SDL_FRect checkRect = { entity->levelX, entity->levelY, entity->destinationRect.w, entity->destinationRect.h };

        // Are we in the camera's view?
        if (!CollisionCheckF(camera, checkRect)) continue;

        // Call the entities render function.
        if (entity->HasTag(Tag_Renderable) || !entity->HasTag(Tag_NotRendering))
        {
            entity->Draw(window, renderer);
        }
    }
}

// Render our level data.
void Resources::RenderLevel(Level* level)
{
    if (!level) return;

    // Offset our collision rectangles by the camera.
    for (auto& obj : level->lCollisionR)
    {
        obj.collisionRect.x = obj.levelX - EngineResources.camera.x;
        obj.collisionRect.y = obj.levelY - EngineResources.camera.y;
    }

    // There can be up to 16 different layers for both entities and tiles. To allow for
    // tiles to overlap entities (and vice versa), we'll render the tile layer FIRST and then
    // render entities from the bottom 0 layer to the top layer 15.
    for (int i = 0; i < MAX_TILE_LAYERS; i++)
    {
        // Render our tiles.
        for (auto & tile : level->lTiles[i])
        {
            // Manipulate the destinationRect of this tile to be offset by the camera.
            tile->destinationRect.x = tile->levelX - camera.x;
            tile->destinationRect.y = tile->levelY - camera.y;

            SDL_FRect checkRect = { tile->levelX, tile->levelY, tile->destinationRect.w, tile->destinationRect.h };

            // Are we in the camera's view?
            if (!CollisionCheckF(camera, checkRect)) continue;

            // Call the tiles' render function.
            if (tile->HasTag(Tag_Renderable) || !tile->HasTag(Tag_NotRendering))
            {
                tile->Draw(window, renderer);
            }
        }

        // Render our entities.
        for (auto& entity : level->lEntities[i])
        {
            // Manipulate the destinationRect of this tile to be offset by the camera.
            entity->destinationRect.x = entity->levelX - camera.x;
            entity->destinationRect.y = entity->levelY - camera.y;

            SDL_FRect checkRect = { entity->levelX, entity->levelY, entity->destinationRect.w, entity->destinationRect.h };

            // Are we in the camera's view?
            if (!CollisionCheckF(camera, checkRect)) continue;

            // Call the entities render function.
            if (entity->HasTag(Tag_Renderable) || !entity->HasTag(Tag_NotRendering))
            {
                entity->Draw(window, renderer);
            }
        }
    }
    return;
}

// Render any other misc things in this function here.
void Resources::RenderMisc()
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
void Resources::FinishRender()
{
    SDL_RenderPresent( renderer );
}