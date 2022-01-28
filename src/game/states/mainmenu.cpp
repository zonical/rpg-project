#include "rpg/engine.h"
#include "rpg/gui/menus/basemenu.h"
#include "rpg/states/mainmenu.h"
#include "rpg/states/overworld.h"

// Create our inital menu.
MainMenuState::MainMenuState()
{
    GameEngine->states.push_back(this);
}

// Update everything in this state including entities and the GUI.
void MainMenuState::Update(float dT)
{
    for (auto& guiLayer : gGUI.elements)
    {
        for (auto& element : guiLayer)
        {
            // Wake them up.
            element->Update(dT);
        }
    }
}

// Draw our GUI elements.
void MainMenuState::Draw(SDL_Window* win, SDL_Renderer* ren)
{
    // Wake up our GUI elements.
    for (auto& guiLayer : gGUI.elements)
    {
        for (auto& element : guiLayer)
        {
            // Wake them up.
            element->Draw(win, ren);
        }
    }
}

// We're only going to process keyboard inputs for our active menu.
void MainMenuState::OnKeyboardInput(SDL_Keycode keyCode, bool pressed, bool released, bool repeat)
{
    for (auto& guiLayer : gGUI.elements)
    {
        for (auto& element : guiLayer)
        {
            // Wake them up.
            element->OnKeyboardInput(keyCode, pressed, released, repeat);
        }
    }
}

// Create a new menu.
void MainMenuState::OnStateActivated()
{
    // If you're wondering why we create a menu here instead of making one in the constructor,
    // we'd get a read violation error when creating an element and inserting it into the GUI.
    // This is a safer way of doing that as everything up to when the state is initalized has
    // been constructed.
    if (!mainmenu.get())
    {
        std::shared_ptr<BaseMenu> menu(new BaseMenu(this->gGUI.FindFirstFreeLayer(), "mainmenu"));
        mainmenu = menu;
        this->gGUI.AddElement(menu, menu->guiLayer);
        menu->OnElementSpawned();

        // Create our title element.
        TextSettings textsettings;
        textsettings.text = "RPG Project - Build: " + std::string(__DATE__) + " " + std::string(__TIME__);
        textsettings.textSize = 48;
        textsettings.color = { 255, 255, 255, 255 };

        // Create pointer.
        std::shared_ptr<Text> title(new Text(this->gGUI.FindFirstFreeLayer(), "mainmenu_title", textsettings));
        this->gGUI.AddElement(title, title->guiLayer);
        title->OnElementSpawned();

        // Create a button that allows us to enter into the debug room. Initalizes the Overworld state
        // and sets level to the debug room.
        auto debugroomText = mainmenu->AddMenuOption("Enter Debug Room", [this]()
            {
                State_Overworld->ResetState();
                State_Overworld->gLevel = new Level();
                State_Overworld->gLevel->LoadLevel("assets/levels/debug_room.json");
                GameEngine->ChangeGameState(State_Overworld);
            });

        // Set the position of our button.
        debugroomText->destinationRect.x = 100;
        debugroomText->destinationRect.y = 100;

        // Create a button that toggles the debug mode state.
        auto debugmodeText = mainmenu->AddMenuOption("Toggle Debug Mode", [this]()
            {
                GameEngine->debugModeEnabled = !GameEngine->debugModeEnabled;
            });

        
        // Set the position of our button.
        debugmodeText->destinationRect.x = 100;
        debugmodeText->destinationRect.y = 140;

        // Add our elements.
        gGUI.AddElement(debugroomText, debugroomText->guiLayer);    // Debug room button.
        gGUI.AddElement(debugmodeText, debugmodeText->guiLayer);    // Debug mode toggle button.
    }
 
}

void MainMenuState::OnStateDeactivated()
{

}

// Destroy our menu.
void MainMenuState::ClearMenuElements()
{
    // Loop over our menu options and destroy them.
    std::for_each(mainmenu->textOptions.begin(), mainmenu->textOptions.end(),
        [this](std::shared_ptr<Text> const& value) {
            gGUI.RemoveElement(value, value->guiLayer);
        });
    mainmenu->textOptions.clear();
}
