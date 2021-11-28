#pragma once
#ifndef TILESETMANAGER_H
#define TILESETMANAGER_H

#include "SDL/SDL.h"
#include "rpg/level/tileset.h"
#include <string>
#include <map>

// Responsible for all of our tilesets.
class TilesetManager
{
private:
	// An internal map contianing all of our tilesets.
	std::map<std::string, Tileset> tilesets;
public:
	void Initalize();

	// Releases all tilesets from memory.
	void ReleaseAllTilesets();

	// Loads a tileset and stores it internally.
	bool			LoadTileset(std::string texture);
	// Boolean check to see if a tileset exists.
	bool			TilesetExists(std::string texture);
	// Removes a tileset and frees it.
	bool			RemoveTileset(std::string texture);
	// Returns a tileset.
	Tileset			GetTileset(std::string texture);
};
#endif