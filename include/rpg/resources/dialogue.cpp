#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "rpg/engine.h"
#include "rpg/resources/dialoguemanager.h"
#include <string>
#include <map>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

void DialogueManager::Initalize()
{
    std::string path = "assets/scripts/text";
    for (const auto& entry : fs::recursive_directory_iterator(path))
    {
        // Get the path as a string.
        std::string path = entry.path().string();

        // Replace the slashes in our path with forward slashes.
        int stringIndex;
        while ((stringIndex = path.find("\\")) != std::string::npos)
        {
            path.replace(stringIndex, std::string("/").length(), std::string("/"));
        }

        // We only load JSON files. This check is here as folders count under the iterator
        // and we obviously can't load a folder.
        if (path.ends_with(".json")) LoadDialogue(path);
    }
}

std::vector < DialogueMessage > DialogueManager::GetDialogue(std::string dialoguePath)
{
    // Double check to see if this dialogue exists.
    if (!DialogueExists(dialoguePath)) return std::vector<DialogueMessage>();

    // Find our dialogue with the dialogue path.
    auto iter = dialogue.find(dialoguePath);
    return iter->second;
}

bool DialogueManager::DialogueExists(std::string dialoguePath)
{
    // Find our dialogue with the dialogue path.
    auto iter = dialogue.find(dialoguePath);
    return !(iter == dialogue.end());
}

bool DialogueManager::RemoveDialogue(std::string dialoguePath)
{
    // Double check to see if this dialogue exists.
    if (!DialogueExists(dialoguePath)) return false;

    // Remove this dialogue from the map.
    dialogue.erase(dialoguePath);
    return true;
}

bool DialogueManager::LoadDialogue(std::string dialoguePath)
{
    try
    {
        std::vector < DialogueMessage > messages;

        // Load our dialogue file.
        using json = nlohmann::json;
        auto dialogue = GameEngine->LoadJSON(dialoguePath);

        // Grab our messages. This is stored as an array of objects.
        auto jsonMessages = dialogue["messages"].get<std::vector<json>>();

        // Loop through all of our messages and store them internally.
        for (auto& message : jsonMessages)
        {
            // Create a struct that will store our data.
            DialogueMessage dM;

            // Grab our JSON data.
            dM.textRate = message["text_rate"].get<float>();
            dM.text = message["text_message"].get<std::string>();
            dM.canSkip = message["can_skip"].get<bool>();

            // Store it.
            messages.push_back(dM);
        }

        // Insert it into our list of dialogues.
        this->dialogue.insert(std::make_pair(dialoguePath, messages));
        printf("[DIALOGUE] Loaded file %s containing %d messages.\n", dialoguePath.c_str(), messages.size());
    }

    // If we run into a processing error, catch here and report.
    catch (std::exception Exception)
    {
        printf("[DIALOGUE] Failed to process %s: %s\n", dialoguePath.c_str(), Exception.what());
        return false;
    }
    return true;
}