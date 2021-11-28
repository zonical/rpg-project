#include <string>
#include <array>
#include <iostream>
#include <fstream>

#include "SDL/SDL.h"
#include "rpg/gui/base.h"
#include "rpg/gui/textbox.h"

#include "json/json.hpp"

Textbox::Textbox(int layer, int textboxType) : GUIElement(layer)
{
    this->AddTag("Textbox");
    // Setup our textbox.

    // This struct is responsible for the settings on our Text element.
    TextSettings textSettings;

    try
    {
        // Load a file containing all of our textbox types.
        using json = nlohmann::json;
        auto textboxTypes = GameEngine->LoadJSON(TEXTBOX_DEFINITIONS_PATH);

        // Grab our textbox type settings.
        auto settings = textboxTypes[std::to_string(textboxType)].get<json>();

        auto padding = settings["text_padding"].get<float>();
        this->borderSize = settings["borderSize"].get<float>();

        // Positioning.
        destinationRect.x = settings["x"].get<float>();
        destinationRect.y = settings["y"].get<float>();
        
        // Width and height of the box.
        destinationRect.w = settings["width"].get<float>();
        destinationRect.h = settings["height"].get<float>();

        // Width and height of the text.
        textSettings.wrappingWidth = settings["width"].get<float>() - this->borderSize - padding;
        //textSettings.fixedHeight = destinationRect.h;

        textSettings.x = settings["x"].get<float>() + this->borderSize + padding;
        textSettings.y = settings["y"].get<float>() + this->borderSize + padding;

        // Grab our background colors.
        auto bgColor = textboxTypes["bgColor"].get<json>();
        this->bgColor.r = bgColor["r"].get<float>();
        this->bgColor.g = bgColor["g"].get<float>();
        this->bgColor.b = bgColor["b"].get<float>();
        this->bgColor.a = bgColor["a"].get<float>();

        // Grab our background colors.
        auto borderColor = textboxTypes["borderColor"].get<json>();
        this->borderColor.r = bgColor["r"].get<float>();
        this->borderColor.g = bgColor["g"].get<float>();
        this->borderColor.b = bgColor["b"].get<float>();
        this->borderColor.a = bgColor["a"].get<float>();
    }
    // If we run into a processing error, catch here and report.
    catch (std::exception &Exception)
    {
        printf("[TEXTBOX] Failed to load textbox types: %s\n", Exception.what());
    }

    boxText = new Text(layer+1, textSettings);
    printf("[TEXTBOX] Loaded textbox %d.\n", textboxType);
}

void Textbox::Update(float dT)
{
    // Have we already displayed all of our messages?
    if (IsCurrentMessageFinished() || IsDialogueFinished()) return;

    currentMessage = messages[messagesCounter];

    // Should we display our next character?
    if (GameEngine->elapsedTime < nextUpdate) return;

    // Add a new character to our text.
    currText += currentMessage.text[charCounter];
    boxText->SetText(currText);
    charCounter++;

    nextUpdate = GameEngine->elapsedTime + currentMessage.textRate;
}

void Textbox::ResetText()
{
    currText = "";
    charCounter = 0;
    boxText->SetText("");
}

void Textbox::OnKeyboardInput(SDL_Keycode keyCode, bool pressed, bool released, bool repeat)
{
    if (pressed)
    {
        switch (keyCode)
        {
            case SDLK_RETURN: 
            {
                // If we've finished this message, reset our text to nothing.
                if (IsCurrentMessageFinished())
                {
                    ResetText();
                    messagesCounter++;
                    
                    // If we have no more messages, clean our messages vector.
                    if (IsDialogueFinished()) messages.clear();

                    break;
                }

            }
        }
    }
};

// Checks to see if the size of the text that we're currently displaying on screen
// matches the size of the text we have for this message.
bool Textbox::IsCurrentMessageFinished()
{
    return currText.size() >= currentMessage.text.size();
}

// Checks to see if we've reached the end of our messages with our message counter
// equalling the amount of messages we have.
bool Textbox::IsDialogueFinished()
{
    return messagesCounter >= messages.size();
}


void Textbox::DrawElement(SDL_Window* win, SDL_Renderer* ren)
{
    // Draw our background box.
    SDL_SetRenderDrawColor(ren, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRectF(ren, &destinationRect);

    // 0 is top, 1 is bottom, 2 is left, 3 is right.
    std::array<SDL_FRect, 4> rects{};
    rects.fill(destinationRect);

    // Construct our outlines.
    rects[0].h = borderSize;
    rects[1].h = borderSize;
    rects[1].y = destinationRect.y + destinationRect.h - borderSize; // acount for border.
    rects[2].w = borderSize;
    rects[3].w = borderSize;
    rects[3].x = destinationRect.x + destinationRect.w - borderSize; // acount for border.

    SDL_SetRenderDrawColor(ren, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    SDL_RenderFillRectF(ren, &rects[0]);
    SDL_RenderFillRectF(ren, &rects[1]);
    SDL_RenderFillRectF(ren, &rects[2]);
    SDL_RenderFillRectF(ren, &rects[3]);
    
    return;
}