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
    gRenderer.Initalize();

    // Load everything from our manifest files. These hold references to all of our fonts
    // and textures. We'll start with fonts.
    using json = nlohmann::json;
   
    try
    {
        // Load the file and have it automatically be parsed by the engine.
        auto fontsFile = LoadJSON("assets/scripts/font_manifest.json");

        // Iterate over each element.
        for (auto& el : fontsFile.items())
        {
            // Grab the key name.
            auto key = el.key();
            auto fontDefinition = el.value().get<json>();

            // Our font definition is another JSON object. It contains a few primary things:
            // The path to the file of the font from "assets/fonts..." and including the extension
            // ".ttf". It also contains an array of sizes since we can't dynamically size these fonts
            // without creating textures for every single size.
            auto fontFilePath = fontDefinition["file"].get<std::string>();
            auto fontSizes = fontDefinition["sizes"].get<std::vector<int>>();   // An array of integers.

            // For each of our sizes, create a new font and store it internally.
            for (auto& size : fontSizes) 
            {
                // Load the font from the file.
                gRenderer.LoadFont(key, fontFilePath, size);
            }

        }

        // Next we'll load all of our textures. This also comes from a manifest file.
        auto texturesFile = LoadJSON("assets/scripts/texture_manifest.json");
        auto listOfTextures = texturesFile["textures"].get<std::vector<std::string>>(); // An array of strings.

        // For all of textures in this array, load them.
        for (auto& texture : listOfTextures)
        {
            // Load the font from the file.
            gRenderer.LoadTexture(texture);
        }

    }
    catch (std::exception& exception)
    {
        printf("Failed to initalize Engine. %s\n", exception.what());
        return false;
    }
    return true;
}

void Engine::Shutdown()
{
    // Free all of our renderer resources.
    gRenderer.Shutdown();

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

#ifdef _DEBUG
    TextSettings debugTextSettings;
    debugTextSettings.wrappingWidth = DEFAULT_SCREEN_WIDTH;
    debugTextSettings.fixedHeight = 250;

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
            //<< "Player Pawn xPos: " << floor(player.Pawn()->x()) << '\n'
            //<< "Player Pawn yPos: " << floor(player.Pawn()->y()) << '\n'
            << "Frame: " << frame << '\n'
            << "Elapsed Time: " << elapsedTime << '\n'
            << "Delta Time: " << deltaTime << '\n'
            << "Entity Count: " << gEntities.size() << '\n'
            << "GUI Element Count: " << guiElementCount << '\n'
            ;

        debugText->SetText(fpsText.str());
#endif

        frame++;
        Uint32 start = SDL_GetTicks();

        // Update all of the entities and stuff in our world.
        Update(deltaTime);

        // Render everything.
        gRenderer.OnPreRender();                    // Prepare rendering stuff.
        gRenderer.RenderEntities(gEntities);        // Render our entities.
        gRenderer.RenderMisc();                     // Render anything else.
        gRenderer.FinishRender();                   // Finish rendering.

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
}