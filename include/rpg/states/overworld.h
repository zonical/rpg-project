#pragma once
#ifndef OVERWORLD_STATE_H
#define OVERWORLD_STATE_H

// State that represents the Overworld.
#include "rpg/level/level.h"
#include "rpg/gamestate.h"
#include "rpg/gui/base.h"

enum DEBUG_SELECTION_TYPE
{
	SELECT_LIGHTING,
	SELECT_TILECULLING
};

enum DEBUG_LIGHTING_TYPE
{
	LIGHTING_STANDARD,
	LIGHTING_DISABLED,
	LIGHTING_BORDER,
	LIGHTING_FILL
};

enum DEBUG_TILECULLING_TYPE
{
	CULLING_STANDARD,
	CULLING_NO_LIGHTS,
	CULLING_DISABLED
};

class OverworldState : public BaseGameState
{
public:
	OverworldState();

	// The current game level.
	Level* gLevel;

	// Data for the level transitions.
	LevelTransitionData gLevelTransData;

	// Level functions.
	void        OnLevelLoaded();
	void        OnLevelShutdown();

	// Transitions to the level listed in gLevelTransData.
	void        PerformLevelTransition();

	// Name of our state.
	std::string name = "OverworldState";

	// When this game state is activated.
	void OnStateActivated() { BaseGameState::OnStateActivated(); };

	// When this game state is deactivated.
	void OnStateDeactivated()
	{
	};

	// The camera that everything revolves around.
	SDL_FRect camera;

	// Offsets the camera by a certain amount.
	void OffsetCamera(int x, int y);

	// Debug.
	int tilesRendered = 0;
	int selection = SELECT_LIGHTING;
	int lightingRenderType = LIGHTING_STANDARD;
	int tileCullingType = CULLING_STANDARD;

	void ResetState()
	{
		delete gLevel;
		for (auto& layer : gGUI.elements) { layer.clear(); }
	};

	// Update function for this state, ran every frame.
	void Update(float dT);

	// Drawing function for our state.
	void Draw(SDL_Window* win, SDL_Renderer* ren);

	// If we get a keyboard event, intercept it and pass it onto our GUI and entities.
	void OnKeyboardInput(SDL_Keycode keyCode, bool pressed, bool released, bool repeat);

	~OverworldState()
	{
		
	}

	// The main instance of this state that is globally accessable.
	static OverworldState* instance()
	{
		static OverworldState instance;
		return &instance;
	};
};

#endif