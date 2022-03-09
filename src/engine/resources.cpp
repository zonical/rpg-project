#include "rpg/rpg.h"

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
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

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

// Render our level data.
void Resources::RenderState(BaseGameState* state)
{
    state->Draw(window, renderer);
}

// Render any other misc things in this function here.
void Resources::RenderMisc()
{
    // We're currently attempting to perform a fade.
    if (this->currentlyFading)
    {
        switch (fadeType)
        {
            // Fading from black to the scene.
            case FADE_FROMBLACK:
            {
                fadeProgress += -fadeDelta;
                fadeColor.a = std::max((int)fadeProgress, 0);

                // If our fade is zero, stop fading.
                if (fadeColor.a == 0)
                {
                    this->currentlyFading = false;
                    fadeType = -1;
                }
                break;
            }
            // Fading from black to the scene.
            case FADE_TOBLACK:
            {
                fadeProgress += fadeDelta;
                fadeColor.a = std::min((int)fadeProgress, 255);

                // If our fade is zero, stop fading.
                if (fadeColor.a == 255)
                {
                    this->currentlyFading = false;
                    fadeType = -1;
                }
                break;
            }
        }
    }
    SDL_SetRenderDrawColor(renderer, fadeColor.r, fadeColor.g, fadeColor.b, fadeColor.a);
    SDL_RenderFillRect(renderer, &fadeRect);
}

// Finish rendering to the screen by actually renderign.
void Resources::FinishRender()
{
    SDL_RenderPresent( renderer );
}

void Resources::FadeFromBlack(float duration)
{
    // Are we currently fading?
    if (this->currentlyFading) return;

    // Set our SDL_Rect color to be completely black.
    fadeColor = { 0, 0, 0, 255 };

    // Get the fade delta, or transparency we should remove on each iteration.
    // Duration is in milliseconds.
    fadeDelta = 255 / duration;

    currentlyFading = true;
    fadeType = FADE_FROMBLACK;

    // Our renderer will handle the rest from there.
    return;
}

void Resources::FadeToBlack(float duration)
{
    // Are we currently fading?
    if (this->currentlyFading) return;

    // Set our SDL_Rect color to be completely black.
    fadeColor = { 0, 0, 0, 0 };

    // Get the fade delta, or transparency we should remove on each iteration.
    // Duration is in milliseconds.
    fadeDelta = 255 / duration;

    currentlyFading = true;
    fadeType = FADE_TOBLACK;

    // Our renderer will handle the rest from there.
    return;
}

void Resources::ResetFade()
{
    // Reset our fade.
    fadeColor = { 0, 0, 0, 0 };
    currentlyFading = false;
    fadeDelta = 0;
    fadeType = -1;

    // Our renderer should handle the rest.
    return;
}

void Resources::ApplyGlobalFade(SDL_Color color)
{
    // Applies a basic faded effect over the entire screen.
    fadeColor = color;
    return;
}
