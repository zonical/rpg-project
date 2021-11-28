#pragma once
#ifndef DIALOGUEMANAGER_H
#define DIALOGUEMANAGER_H

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

#include <string>
#include <map>
#include <vector>

struct DialogueMessage
{
    // The text that this message contains.
    std::string text = "";

    // How long to wait after each character is displayed before displaying the next
    // in milliseconds.
    float textRate = 0.0;

    // Can this text be automatically skipped?
    bool canSkip = false;
};

// QUICK TERMINOLOGY CHECK:
// Dialogue     - A collection of messages, all defined in one file.
// Message      - One message that gets shown in a textbox, has properties like text, delay between
//                characters, etc.

// Responsible for all of our dialogue.
class DialogueManager
{
private:
    // Map of all of our dialogue. The first value is the filepath to the dialogue script
    // and the second value is a vector of DialogueMessages parsed from the JSON file.
    std::map<std::string, std::vector<DialogueMessage>> dialogue;

public:
    void Initalize();

    // Loads a dialogue script by file path.
    bool			    LoadDialogue(std::string dialoguePath);
    // Boolean check to see if a texture exists.
    bool			    DialogueExists(std::string dialoguePath);
    // Removes a vector of dialogue.
    bool			    RemoveDialogue(std::string dialoguePath);
    // Grabs a vector of dialogue messages.
    std::vector
    <DialogueMessage>   GetDialogue(std::string dialoguePath);
};

#endif