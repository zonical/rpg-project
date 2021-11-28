#pragma once
#ifndef TILESET_H
#define TILESET_H

#include "SDL/SDL.h"
#include <list>
#include <string>

// Represents an individual tile in the tileset.
struct TileData
{
	// The texture for this tile.
	SDL_Texture* tileTexture;

	// Image rect of this tile, a 64x64 space.
	SDL_Rect rect = { 0, 0, 64, 64 };
};

// Represents an internal tileset.
struct Tileset
{
	// The image of our tileset.
	SDL_Texture* textureTiles;

	// Our list of tiles.
	std::list < TileData > tiles;

	// Releases all of our tile textures.
	void FreeTiles()
	{
		for (auto& tile : tiles)
		{
			SDL_DestroyTexture(tile.tileTexture);
		}
		SDL_DestroyTexture(textureTiles);
	}
};

#endif