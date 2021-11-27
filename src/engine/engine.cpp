#include "rpg/engine.h"
#include "rpg/renderer.h"
#include "rpg/entity.h"
#include "rpg/entities/character.h"
#include "rpg/taggable.h"

#include <string>
#include <fstream>
#include <iostream>

Engine* Engine::instance()
{
    static Engine instance;
    return &instance;
}

// Updates all of our dynamic elements in the engine every frame with the
// deltaTime from the last frame. This mainly consists of polling events,
// updating entities and updating GUI elemnts.
void Engine::Update(double dT)
{
    SDL_Event event;
    // Poll events.
    SDL_PollEvent(&event);

    switch (event.type)
    {
        // We're quitting the main application, stop the logic loop.
        case SDL_QUIT: 
        {
            stopping = true;
            break;
        }
            
        case SDL_KEYDOWN:
        {
            // Tell all of our entities that a key has been pressed.
            for (auto& entity : gEntities)
            {
                // Call the entities keyboard input function.
                entity->OnKeyboardInput(event.key.keysym.sym, true, false, !(event.key.repeat));
            }

            // Tell all of our elements that a key has been pressed.
            for (auto& layer : gGUI.elements)
            {
                for (auto& element : layer)
                {
                    // Call the elements keyboard input function.
                    element->OnKeyboardInput(event.key.keysym.sym, true, false, !(event.key.repeat));
                }
            }
            break;
        }

        case SDL_KEYUP:
        {
            // Tell all of our entities that a key has been released.
            for (auto& entity : gEntities)
            {
                // Call the entities keyboard input function.
                entity->OnKeyboardInput(event.key.keysym.sym, false, true, !(event.key.repeat));
            }

            // Tell all of our elements that a key has been released.
            for (auto& layer : gGUI.elements)
            {
                for (auto& element : layer)
                {
                    // Call the elements keyboard input function.
                    element->OnKeyboardInput(event.key.keysym.sym, false, true, !(event.key.repeat));
                }
            }
            break;
        }
    }

    // Run logic for all of our entities.
    for (auto& entity : gEntities)
    {
        entity->Update(dT);
    }

    // Run logic for all of our GUI elements.
    for (auto& layer : gGUI.elements)
    {
        for (auto& element : layer)
        {
            element->Update(dT);
        }
    }
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
        return jsonObj;
    }
    catch (std::exception& exception)
    {
        throw exception;
        
        // Return an empty object.
        return json::object();
    }
}