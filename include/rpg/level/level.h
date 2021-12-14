#pragma once
#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <array>

#include "SDL/SDL.h"

#include "rpg/entity.h"
#include "rpg/entities/character.h"
#include "rpg/level/tile.h"
#include "rpg/base/renderable.h"
#include "rpg/base/taggable.h"
#include <memory>

struct CollisionRect
{
	SDL_FRect collisionRect;
	float levelX, levelY;
};

struct LevelTransitionData
{
	bool transitionFlag = false;
	std::string landmark_name;
	std::string new_level;
};

class Level
{
public:
	Level();
	~Level();

	// Grab the character that we control.
	Character* GetCharacter();

	// A list of this levels local entities. This list gets completely scrapped
	// when this level is unloaded.
	std::array < std::vector < std::unique_ptr<Entity> >, MAX_TILE_LAYERS >  lEntities;

	// We support up to 16 layers of tiles at one time. Each layer consists of a vector of Tile pointers.
	std::array < std::vector < std::unique_ptr<Tile> >, MAX_TILE_LAYERS > lTiles;

	// All of the rectangles that we can collide with.
	std::vector < CollisionRect > lCollisionR;

	using json = nlohmann::json;

	// Load the level with it's tile and entity data.
	bool LoadLevel(std::string levelPath);
	bool CreateTiles(json levelData);
	bool CreateEntities(json levelData);
	bool CreateCollision(json levelData);

	void LevelUpdate(float dT);

	// Functions to help with loading levels.
	std::string CleanupSourceImage(std::string string);

	// Free's all of our entities and tiles.
	void FreeResources();
};

#endif // !LEVEL_H