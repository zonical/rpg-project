#include "rpg/rpg.h"
#include "rpg/gui/menus/basemenu.h"
#include "rpg/states/mainmenu.h"
#include "rpg/states/overworld.h"

// Create our inital menu.
MainMenuState::MainMenuState()
{
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

        // Create our title element.
        std::shared_ptr<Text> title(new Text(this->gGUI.FindFirstFreeLayer(), "mainmenu_title"));
        title->SetTextColor({ 255, 255, 255, 255 });
        title->SetFont(EngineResources.fonts.GetFont("assets/fonts/40573_VIDEOTER", 48));
        title->SetText("RPG Project - Build: " + std::string(__DATE__) + " " + std::string(__TIME__));
        title->destinationRect.x = 20;
        title->destinationRect.y = 20;

        // Create a button that allows us to enter into the debug room. Initalizes the Overworld state
        // and sets level to the debug room.
        this->enterDebugRoomText = mainmenu->AddMenuOption("Enter Debug Room", [this]()
            {
                GameEngine->GetOverworldState()->ResetState();
                GameEngine->GetOverworldState()->gLevel = new Level();
                GameEngine->GetOverworldState()->gLevel->LoadLevel("assets/levels/debug_room.json");
                GameEngine->ChangeGameState(GameEngine->GetOverworldState());
            });

        // Set the position of our button.
        this->enterDebugRoomText->destinationRect.x = 100;
        this->enterDebugRoomText->destinationRect.y = 100;
        this->enterDebugRoomText->SetTextColor({ 255, 255, 255, 255 });

        // Create a button that toggles the debug mode state.
        this->toggleDebugModeText = mainmenu->AddMenuOption("Toggle Debug Mode", [this]()
            {
                GameEngine->debugModeEnabled = !GameEngine->debugModeEnabled;

                // If we're enabled, change our text to green and say "Enabled"
                if (GameEngine->debugModeEnabled)
                {
                    this->toggleDebugModeVisualText->SetTextColor({ 0, 255, 0, 255 });
                    this->toggleDebugModeVisualText->SetText("Enabled");
                }
                // If we're disabled, change our text to red and say "Disabled"
                else
                {
                    this->toggleDebugModeVisualText->SetTextColor({ 255, 0, 0, 255 });
                    this->toggleDebugModeVisualText->SetText("Disabled");
                }
                
            });

        // Set the position of our button.
        this->toggleDebugModeText->destinationRect.x = 100;
        this->toggleDebugModeText->destinationRect.y = 140;
        this->toggleDebugModeText->SetTextColor({ 255, 255, 255, 255 });

        // Add special text showing the state of the debug mode.
        this->toggleDebugModeVisualText = std::shared_ptr<Text>(new Text(this->toggleDebugModeText->guiLayer, "debugtext_mode"));
        this->toggleDebugModeVisualText->SetTextColor({ 255, 0, 0, 255 });
        this->toggleDebugModeVisualText->SetFont(EngineResources.fonts.GetFont("40573_VIDEOTER", 48));
        this->toggleDebugModeVisualText->SetText("Disabled");
        this->toggleDebugModeVisualText->destinationRect.x = 200;
        this->toggleDebugModeVisualText->destinationRect.y = 140;

        // Add our elements.
        gGUI.AddElement(title, title->guiLayer);
        gGUI.AddElement(this->enterDebugRoomText, this->enterDebugRoomText->guiLayer);    // Debug room button.
        gGUI.AddElement(this->toggleDebugModeText, this->toggleDebugModeText->guiLayer);    // Debug mode toggle button.
        gGUI.AddElement(this->toggleDebugModeVisualText, this->toggleDebugModeVisualText->guiLayer);    // Debug mode toggle button.
    }
    BaseGameState::OnStateActivated();
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
