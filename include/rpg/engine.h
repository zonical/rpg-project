#ifndef HEADER_H_ENGINE
#define HEADER_H_ENGINE

#include "SDL/SDL.h"
#include "json/json.hpp"

#include "rpg/resources.h"
#include "rpg/entity.h"
#include "rpg/level/level.h"
#include "rpg/gui/base.h"

#include <stdio.h>

enum GameState
{
    GAMESTATE_LOADING = 1,
    GAMESTATE_TITLESCREEN,
    GAMESTATE_OVERWORLD,
    GAMESTATE_BATTLE,
    GAMESTATE_PAUSED
};

class Engine
{
private:
    // The current game state.
    static int currentGameState;

    // If we're stopping the main gameplay loop.
    bool stopping = false;

    // Update everything.
    void Update(double dT);

public:
    // The main instance of the Engine that is globally accessable.
    static      Engine* instance();
    
    bool        Initalize();        // The main function that initalizes all of our systems.
    void        Shutdown();         // The function that shuts down all of our systems and frees resources.
    void        MainLoop();         // The main game loop.
    void        SetGameState();     // Set our current game state and call certain update functions.

    void        RegisterGlobalEntity(Entity* ent);    // Inserts an entity into the global list.
    void        RemoveGlobalEntity(Entity* ent);      // Removes an entity from the global list.

    // Global objects that are split into different classes.
    std::vector< Entity* > gEntities;   // A vector of all global entities in the world.
    Resources    gResources;             // Rendering system object.
    GUI         gGUI;                   // Global GUI handler.
    
    double      elapsedTime;            // Time elapsed in 1/10ths of a second.

    using json = nlohmann::json;
    json        LoadJSON(std::string file);     // Load and parse JSON file.
};

#define GameEngine \
    Engine::instance()
    
#define EngineResources \
    GameEngine->gResources

#endif