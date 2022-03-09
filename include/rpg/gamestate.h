#pragma once
#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>
#include <iostream>
#include "rpg/gui/base.h"
#include "rpg/base/renderable.h"
#include "SDL/SDL.h"

// The base class that represents a game state.
class BaseGameState : public Renderable, Inputtable
{
public:
	// Name of this state.
	std::string name;

	// The GUI for this state.
	GUI gGUI;

	// When this game state is activated.
	virtual void OnStateActivated()
	{
		std::cout << "[ENGINE] Activated state: " << this->name << std::endl;

		// Activate all of our elements.
		for (auto& layer : this->gGUI.elements)
		{
			for (auto& element : layer)
			{
				element->OnElementSpawned();
			}
		}
	};

	// When this game state is deactivated.
	virtual void OnStateDeactivated()
	{
		std::cout << "[ENGINE] Deactivated state: " << this->name << std::endl;
	};

	virtual void ResetState() {};

	// Update function for this state, ran every frame.
	virtual void Update(float dT) {};

	// If we get a keyboard event, intercept it and pass it onto our GUI and entities.
	virtual void OnKeyboardInput(SDL_Keycode keyCode, bool pressed, bool released, bool repeat) {}
	virtual void OnMouseWheelScrolled(int x, int y, int direction) {};

	// Rendering function for our state.
	virtual void Draw(SDL_Window* win, SDL_Renderer* ren) {};
};

#endif GAMESTATE_H