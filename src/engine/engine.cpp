#include "rpg/engine.h"
#include "rpg/resources.h"
#include "rpg/entity.h"
#include "rpg/entities/character.h"
#include "rpg/base/taggable.h"

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

    while (SDL_PollEvent(&event))
    {
        // If we're being marked for level transition, delete our current level and start transitioning
        // to the next one. Don't update anything else here.
        if (gLevel != nullptr && gLevelTransData.transitionFlag)
        {
            PerformLevelTransition();
            return;
        }

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
                if (gLevel != nullptr)
                {
                    // Tell all of our entities that a key has been pressed.
                    for (auto& entityLayer : gLevel->lEntities)
                    {
                        for (auto& entity : entityLayer)
                        {
                            // Call the entities keyboard input function.
                            entity->OnKeyboardInput(event.key.keysym.sym, true, false, event.key.repeat);
                        }
                    }
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
                if (gLevel != nullptr)
                {
                    // Tell all of our entities that a key has been pressed.
                    for (auto& entityLayer : gLevel->lEntities)
                    {
                        for (auto& entity : entityLayer)
                        {
                            // Call the entities keyboard input function.
                            entity->OnKeyboardInput(event.key.keysym.sym, false, true, !(event.key.repeat));
                        }
                    }
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
    }

    if (gLevel != nullptr)
    {
        gLevel->LevelUpdate(dT);
    }

    // Run logic for all of our GUI elements.
    for (auto& layer : gGUI.elements)
    {
        for (auto& element : layer)
        {
            if (element.get() == nullptr) continue;
            element->Update(dT);
        }
    }
}

// Wake up our entities and GUI elements once everything has been constructed.
void Engine::OnLevelLoaded()
{
    if (this->gLevel == nullptr) return;

    // Wake up our entities.
    for (auto& entityLayer : this->gLevel->lEntities)
    {
        for (auto& entity : entityLayer)
        {
            // Wake them up.
            entity->OnEntitySpawned();
        }
    }
    
    // Wake up our GUI elements.
    for (auto& guiLayer : gGUI.elements)
    {
        for (auto& element : guiLayer)
        {
            // Wake them up.
            element->OnElementSpawned();
        }
    }
}

// If our level is being destroyed, we'll completely remove all screen elements so we can
// have an entirely new slate for the next scene.
void Engine::OnLevelShutdown()
{
    if (this->gLevel == nullptr) return;

    for (auto& guiLayer : this->gGUI.elements)
    {
        guiLayer.clear();
    }
}

// Begin transitioning to our new level. Our gLevelTransData struct holds the information
// we need to begin creating the new level, so we'll begin that straight away.
void Engine::PerformLevelTransition()
{
    // Delete our current level.
    delete gLevel;

    // Create our new level and load in everything we need.
    gLevel = new Level();
    gLevel->LoadLevel(gLevelTransData.new_level);

    // Now that we've loaded in our level, we have access to all of our entities.
    // Find our level landmark and set our player position to be that point.
    for (auto& entLayer : gLevel->lEntities)
    {
        for (auto& entity : entLayer)
        {
            // Is this entity a landmark and has the required name?
            if (entity->classname == "landmark" &&
                entity->targetname == gLevelTransData.landmark_name)
            {
                // Set our Character position to the position of this landmark.
                Character* character = gLevel->GetCharacter();
                if (character != nullptr)
                {
                    // Do a little offset here so our position is at the origin
                    // of the character sprite.
                    character->ForcePosition(
                        entity->levelX - (character->w() / 2),
                        entity->levelY - (character->h() / 2));
                }
                break;
            }
        }
    }

    // Now that we've completed our level transition, reset our struct so we can
    // resume normal gameplay.
    gLevelTransData.transitionFlag = false;
    gLevelTransData.new_level = "";
    gLevelTransData.landmark_name = "";
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