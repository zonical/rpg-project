#ifndef HEADER_H_ENGINE
#define HEADER_H_ENGINE

#include "SDL/SDL.h"
#include "json/json.hpp"

#include "rpg/gamestate.h"
#include "rpg/resources.h"
#include "rpg/entity.h"
#include "rpg/level/level.h"

#include <stdio.h>
#include <string>
#include <iostream>

#define LEVEL_TRANSITION_FADE 25

class Engine
{
private:
    // If we're stopping the main gameplay loop.
    bool stopping = false;
    void        Update(double dT);  // Update everything.

public:
    // The main instance of the Engine that is globally accessable.
    static      Engine* instance();
    // The main function that initalizes all of our systems.
    bool        Initalize();
    // The function that shuts down all of our systems and frees resources.
    void        Shutdown();   
    // The main game loop.
    void        MainLoop();         
    
    // The current gamestate.
    BaseGameState* gameState;
    std::vector<BaseGameState*> states;

    // Change game state. This calls helper functions for activating and deactiviating a state.
    void ChangeGameState(BaseGameState* newState);
    
    // Resource manager object, handles rendering.
    Resources gResources;
    // Time elapsed in 1/10ths of a second.
    double elapsedTime;
    // The amount of frames total.
    int frame;
    // FPS.
    float fps;

    // Whether or not to enable debug features.
    bool debugModeEnabled;

    using json = nlohmann::json;
    json        LoadJSON(std::string file);     // Load and parse JSON file.
};

#define GameEngine \
    Engine::instance()
    
#define EngineResources \
    GameEngine->gResources

#endif