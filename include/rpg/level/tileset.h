#pragma once
#ifndef TILESET_H
#define TILESET_H

#include "SDL/SDL.h"
#include <list>
#include <string>

// Represents an internal tileset.
class Tileset
{
public:
	// A list of tiles as textures. They will be stored by ID.
	std::list<SDL_Texture*> textureTiles;

	// Loads a tileset into the tiles array.
	void LoadTileset(std::string file);

	// Destroys all of the texture tiles.
	void FreeTiles();
};

#endif