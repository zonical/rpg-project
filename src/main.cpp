#define SDL_MAIN_HANDLED

#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <ctime>
#include <list>

#include "SDL/SDL.h"
#include "json/json.hpp"

#include "rpg/engine.h"
#include "rpg/gui/text.h"
#include "rpg/entity.h"
#include "rpg/entities/character.h"

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
    // Free all of our renderer resources.
    gResources.Shutdown();

    // Finally, quit all of our systems.
    TTF_Quit();
    SDL_Quit();
}

void Engine::MainLoop()
{
    // Initalize values keeping time and dealing with frames
    // in this loop.
    int frame = 0;
    int maxFPS = 60;
    double deltaTime = 0;
    float FPS = 0.0f;

    // Get the starting time of when this loop starts. This is used for
    // engine->elapsedTime.
    Uint32 absoluteStartingTime = SDL_GetTicks();

    Character* character = new Character();
    character->AddTag("Moveable");
    RegisterGlobalEntity(character);

#ifdef _DEBUG
    TextSettings debugTextSettings;
    debugTextSettings.wrappingWidth = DEFAULT_SCREEN_WIDTH;
    debugTextSettings.fixedHeight = 350;

    Text* debugText = new Text(MAX_GUI_LAYERS - 1, debugTextSettings);
#endif
    // Main logic loop.
    while (!stopping)
    {
#ifdef _DEBUG
        std::stringstream fpsText;

        int guiElementCount = 0;
        for (auto& layer : gGUI.elements) guiElementCount += layer.size();

        fpsText << "FPS: " << floor(FPS) << '\n'
            << "Frame: " << frame << '\n'
            << "Elapsed Time: " << elapsedTime << '\n'
            << "Delta Time: " << deltaTime << '\n'
            << "Entity Count: " << gEntities.size() << '\n'
            << "GUI Element Count: " << guiElementCount << '\n'
            << "Character World Pos: " << character->levelX << ", " << character->levelY << '\n'
            << "Camera Pos: " << EngineResources.camera.x << ", " << EngineResources.camera.y << '\n'
            ;

        debugText->SetText(fpsText.str());
#endif
        frame++;
        Uint32 start = SDL_GetTicks();

        // Update all of the entities and stuff in our world.
        Update(deltaTime);

        // Render everything.
        gResources.OnPreRender();                    // Prepare rendering stuff.
        gResources.RenderEntities(gEntities);        // Render our entities.
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
        FPS = 1000.0f / deltaTime;

        // Calculate the starting time since initalization.
        elapsedTime = (SDL_GetTicks() - absoluteStartingTime) / 100.0f;
    }
#ifdef _DEBUG
    delete debugText;
#endif

    RemoveGlobalEntity(character);
    delete character;
}