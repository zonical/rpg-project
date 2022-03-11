#pragma once
#ifndef MAINMENU_STATE_H
#define MAINMENU_STATE_H

// State that represents the main menu.
#include "rpg/gamestate.h"
#include "rpg/gui/base.h"
#include "rpg/gui/menus/basemenu.h"

enum MENU_STATE
{
	DEBUG_MENU,
};

class MainMenuState : public BaseGameState
{
public:
	// Name of our state.
	std::string name = "MainMenuState";
	std::shared_ptr<BaseMenu> mainmenu;

	// Hold onto some elements for later.
	std::shared_ptr<Text> enterDebugRoomText;
	std::shared_ptr<Text> toggleDebugModeText;
	std::shared_ptr<Text> toggleDebugModeVisualText;

	MainMenuState();

	// When this game state is activated.
	void OnStateActivated();

	// When this game state is deactivated.
	void OnStateDeactivated();

	void ClearMenuElements();

	// When everything in this state needs to be reset.
	void ResetState()
	{
		ClearMenuElements();
		for (auto& layer : gGUI.elements) { layer.clear(); }
	};

	// Update function for this state, ran every frame.
	void Update(float dT);

	// Drawing function for our state.
	void Draw(SDL_Window* win, SDL_Renderer* ren);

	// If we get a keyboard event, intercept it and pass it onto our GUI and entities.
	void OnKeyboardInput(SDL_Keycode keyCode, bool pressed, bool released, bool repeat);

	// The main instance of this state that is globally accessable.
	static MainMenuState* instance()
	{
		static MainMenuState instance;
		return &instance;
	};

	~MainMenuState()
	{
		/*this->enterDebugRoomText.reset();
		this->toggleDebugModeText.reset();
		this->toggleDebugModeVisualText.reset();*/
		//ClearMenuElements();
	}
};

#endif
