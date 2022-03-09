#define SDL_MAIN_HANDLED
#include "rpg/rpg.h"

#include "rpg/states/mainmenu.h"
#include "rpg/gui/menus/basemenu.h"

#include <ctime>

int main( int argc, char* args[] )
{
    printf("[ENGINE] Starting Engine...\n");
    if (!GameEngine->Initalize()) return 1;

    printf("[ENGINE] Starting Main Loop...\n");
    GameEngine->MainLoop();

    printf("[ENGINE] Shutting down Engine...\n");
    GameEngine->Shutdown();
    return 0;
}

// Initalizes EVERYTHING that is required for this game. Returns false if
// something failed to load.
bool Engine::Initalize()
{
    // Initalize random number generation with a seed based on the time of the system clock. 
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Initalize SDL itself.
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return false;

    // Initalize the font library.
    if (TTF_Init() != 0) return false;

    // Initalize everything for our renderer, mainly our window and SDL_Renderer
    // itself. This allows us to load textures and fonts.
    gResources.Initalize();
    gResources.textures.Initalize();
    gResources.fonts.Initalize();
    gResources.dialogue.Initalize();
    gResources.tilesets.Initalize();

    return true;
}

void Engine::Shutdown()
{
    // Free all resources from our states.
    gameState = nullptr;

    for (auto& state : states)
    { 
        state->ResetState();
    }
   
    // Free all of our renderer resources.
    gResources.Shutdown();

    // Finally, quit all of our systems.
    TTF_Quit();
    SDL_Quit();
}

void Engine::MainLoop()
{
    ChangeGameState(State_MainMenu);

    // Initalize values keeping time and dealing with frames
    // in this loop.
    int maxFPS = 60;
    double deltaTime = 0;

    // Get the starting time of when this loop starts. This is used for
    // engine->elapsedTime.
    Uint32 absoluteStartingTime = SDL_GetTicks();

    // Main logic loop.
    while (!stopping)
    {
        frame++;
        Uint32 start = SDL_GetTicks();

        // Update all of the entities and stuff in our world.
        Update(deltaTime);

        // Render everything.
        gResources.OnPreRender();                    // Prepare rendering stuff.
        gResources.RenderState(gameState);     // Render the currnet state.
        gResources.RenderMisc();                     // Render anything else.
        gResources.FinishRender();                   // Finish rendering.

        Uint32 delayTime;

        // Calculate how much time we need to delay the application by.
        if (maxFPS > 0 && ((SDL_GetTicks() - start) < 1000.0 / maxFPS))
        {
            delayTime = (Uint32)((1000.0 / maxFPS) - (SDL_GetTicks() - start));
            SDL_Delay(delayTime);
        }

        // Calculate deltatime and FPS.
        Uint32 end = SDL_GetTicks();
        deltaTime = (float)(end - start);
        fps = 1000.0f / deltaTime;

        // Calculate the starting time since initalization.
        elapsedTime = (SDL_GetTicks() - absoluteStartingTime) / 100.0f;
    }
}