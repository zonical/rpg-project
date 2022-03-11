#ifndef HEADER_H_ENGINE
#define HEADER_H_ENGINE

#include "SDL/SDL.h"
#include "json/json.hpp"

#include "rpg/gamestate.h"
#include "rpg/states/overworld.h"
#include "rpg/states/mainmenu.h"
#include "rpg/resources.h"

#include <stdio.h>
#include <string>
#include <iostream>

#define DefineGameState(State) State* BOOST_PP_CAT(Get, State)() { return State::instance(); }

#define DEFAULT_SCREEN_WIDTH 1200
#define DEFAULT_SCREEN_HEIGHT 700

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
    // Change game state. This calls helper functions for activating and deactiviating a state.
    void ChangeGameState(BaseGameState* newState);

    // Define game states here. These will be globally accessable.
    DefineGameState(OverworldState);
    DefineGameState(MainMenuState);

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

// Python module init function. This is required in every header file for classes that
// might expose C++ functions to Python!
extern "C" BOOST_SYMBOL_EXPORT PyObject * PyInit_engine();

#endif