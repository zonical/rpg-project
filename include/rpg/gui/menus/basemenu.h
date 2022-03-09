#pragma once
#ifndef BASEMENU_H
#define BASEMENU_H

#include "rpg/gui/base.h"
#include "rpg/gui/text.h"

#include <vector>
#include <functional>

typedef std::function<void()> MenuFunction;

// An option on a menu.
struct MenuOption
{
	// String name.
	std::string optionName;

	// When this menu option is selected, this function is called
	// which is passed through.
	MenuFunction function;
};

// Base menu element for the GUI.
class BaseMenu : public GUIElement
{
public:
	BaseMenu();
	BaseMenu(int layer, std::string name);
	~BaseMenu();

	// Accept keyboard inputs. The user can control the menu cursor with the up, down, left and right
	// arrows if need be. We provide a default implementation but this can be overriden if wanted.
	virtual void OnKeyboardInput(SDL_Keycode keyCode, bool pressed, bool released, bool repeat);

	// Add a new option into our menu.
	std::shared_ptr<Text> AddMenuOption(std::string name, MenuFunction function);

	// Grabs a menu option by name.
	Text* GetMenuOption(std::string name);

	// Grabs a menu option by index.
	Text* GetMenuOption(int index);

	// Draw our menu to the screen.
	void Draw(SDL_Window* win, SDL_Renderer* ren);

	// A vector containing Text elements so we can display our menu options.
	std::vector<std::shared_ptr<Text>> textOptions;

	int CurrentlySelectedOption;

private:
	// A vector containing all of our menu options.
	std::vector<MenuOption> options;
};

#endif