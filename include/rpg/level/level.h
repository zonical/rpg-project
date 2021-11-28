#pragma once
#ifndef LEVEL_H
#define LEVEL_H


#include <vector>

#include "SDL/SDL.h"

#include "rpg/engine.h"
#include "rpg/entity.h"
#include "rpg/base/renderable.h"
class Level : public Renderable
{
public:
	Level();
	~Level();

	// A list of this levels local entities. This list gets completely scrapped
	// when this level is unloaded.
	std::vector < Entity* > lEntities;

	// Render the level to the screen, including it's entities.
	void Draw(SDL_Window* win, SDL_Renderer* ren);
};

#endif // !LEVEL_H