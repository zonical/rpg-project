#pragma once
#include <vector>

#include "SDL/SDL.h"

#include "rpg/engine.h"
#include "rpg/entity.h"
class Level
{
public:
	Level();
	~Level();

	// A list of this levels local entities. This list gets completely scrapped
	// when this level is unloaded.
	std::vector < Entity* > lEntities;

	// Render the level to the screen, including it's entities.
	void DrawLevel(SDL_Window* win, SDL_Renderer* ren);
};