#ifndef TEXTBOX_H
#define TEXTBOX_H
#include <string>
#include "SDL/SDL.h"
#include "rpg/gui/base.h"
#include "rpg/gui/text.h"
#include "rpg/engine.h"
#include "rpg/resources/dialoguemanager.h"
#include <memory>

#define TEXTBOX_DEFINITIONS_PATH "assets/scripts/textboxes.json"

class Textbox : public GUIElement 
{
private:
    // Special element settings.
    float borderSize;
    SDL_Color bgColor;
    SDL_Color borderColor;

    // This is the text element that displays what is currently
    // being processed.
    std::shared_ptr<Text> boxText;

    // All of our messages that we've processed from LoadDialogue()
    // Once all messages have been displayed, this gets cleared.
    std::vector<DialogueMessage> messages;

    // The current message struct that we're currently displaying on screen.
    // When a dialogue script file is processed, this will automatically be set
    // to the first message in that script file.
    DialogueMessage currentMessage;

    // This text is what is currently on screen.
    std::string currText;

    // How many characters we've currently processed. This gets reset to zero
    // for every message that we loop through.
    int charCounter = 0;

    // How many messages we have left to go through.
    int messagesCounter = 0;

    // What type of textbox we are.
    int type;

public:
    // Constructors and deconstructors. The layer constructor will call
    // the internal GUIElement(layer) setup.
    Textbox() {};
    Textbox(int layer, std::string elementName, int textboxType);
    ~Textbox()
    {
        ResetText();
        GameEngine->gGUI.RemoveElement(boxText->elementName, boxText->guiLayer);
        this->boxText.reset();
    }

    // Construct our text element when we spawn in.
    void OnElementSpawned();

    // Dialogue text related functions.
    // Resets the internal values relating to the textbox and resets
    // the text element.
    void ResetText();

    // Checks to see if we've reached the end of our messages with our message counter
    // equalling the amount of messages we have.
    bool IsDialogueFinished();
    // Checks to see if the size of the text that we're currently displaying on screen
    // matches the size of the text we have for this message.
    bool IsCurrentMessageFinished();

    // Loads dialogue from a JSON file and processes it. This fills our messages
    // vector with DialogueMessage structs.
    void LoadDialogue(std::string file);

    // Base element related functions.
    void Draw(SDL_Window*, SDL_Renderer*);
    void OnKeyboardInput(SDL_Keycode keyCode, bool pressed, bool released, bool repeat);

    // Updates the text and controls what should be displayed when.
    void Update(float);
};
#endif // !TEXTBOX_H