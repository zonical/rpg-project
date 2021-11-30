#pragma once
#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <array>

#include "SDL/SDL.h"

#include "rpg/entity.h"
#include "rpg/level/tile.h"
#include "rpg/base/renderable.h"
#include "rpg/base/taggable.h"
class Level
{
public:
	Level();
	~Level();

	// A list of this levels local entities. This list gets completely scrapped
	// when this level is unloaded.
	std::vector < Entity* > lEntities;

	// We support up to 16 layers of tiles at one time. Each layer consists of a vector of Tile pointers.
	std::array < std::vector < Tile* >, MAX_TILE_LAYERS > lTiles;

	// Load the level with it's tile and entity data.
	bool LoadLevel(std::string levelPath);

	// Functions to help with loading levels.
	std::string CleanupSourceImage(std::string string);

	// Free's all of our entities and tiles.
	void FreeResources();
};

#endif // !LEVEL_H