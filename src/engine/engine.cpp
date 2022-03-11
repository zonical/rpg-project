#include "rpg/rpg.h"
#include "rpg/states/overworld.h"

Engine* Engine::instance()
{
    static Engine instance;
    return &instance;
}

// Change game state. This calls helper functions for activating and deactiviating a state.
void Engine::ChangeGameState(BaseGameState* newState)
{
    // Check here to see if we're actually in a state.
    if (gameState)
    {
        gameState->OnStateDeactivated();
    }

    // Set our new state and activatei t.
    gameState = newState;
    gameState->OnStateActivated();
}

// Updates all of our dynamic elements in the engine every frame with the
// deltaTime from the last frame. This mainly consists of polling events,
// updating entities and updating GUI elemnts.
void Engine::Update(double dT)
{
    SDL_Event event;
    // Poll events.

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            // We're quitting the main application, stop the logic loop.
            case SDL_QUIT:
            {
                stopping = true;
                break;
            }

            case SDL_KEYDOWN:
            case SDL_KEYUP:
            {
                // Call the gamestate keyboard input function.
                bool pressed = (event.type == SDL_KEYDOWN);
                gameState->OnKeyboardInput(event.key.keysym.sym, pressed, !pressed, event.key.repeat);
            }
            case SDL_MOUSEWHEEL:
            {
                gameState->OnMouseWheelScrolled(event.wheel.x, event.wheel.y, -1);
            }
        }
    }

    gameState->Update(dT);
}

// Loads a text file and parses the contents into JSON. This is mainly useful
// for configuration files.
nlohmann::json Engine::LoadJSON(std::string file)
{
    try
    {
        std::string fileContents;
        std::string tempText;

        // Try to load a file containing all of our textbox types and parse it into JSON.
        std::ifstream fileObject;
        fileObject.open(file);
        while (std::getline(fileObject, tempText))
        {
            // Output the text from the file
            fileContents += tempText;
        }

        // Parse into JSON.
        using json = nlohmann::json;
        auto jsonObj = json::parse(fileContents);
        fileObject.close();
        return jsonObj;
    }
    catch (std::exception& exception)
    {
        throw exception;
        
        // Return an empty object.
        return json::object();
    }
}

// Python module init for the Engine.
BOOST_PYTHON_MODULE(engine)
{
    using namespace boost;

    // Define all of our OverworldState functions that we're exposing to Python.
    python::class_<OverworldState>("OverworldState")
        .def("OffsetCamera", &OverworldState::OffsetCamera, python::args("x", "y"))
        ;

    // Define the Engine class. This will encompass everything that we've defined up to this point.
    python::class_<Engine>("Engine")
        // Uhhh, according to https://wiki.python.org/moin/boost.python/CallPolicy#reference_existing_object
        // this might be dangerous? Let's better hope not.
        .def("OverworldState", &Engine::GetOverworldState, python::args("lvalue")=GameEngine,
            python::return_value_policy<python::reference_existing_object>());
}