#pragma once
#ifndef TILESET_H
#define TILESET_H

#include "SDL/SDL.h"
#include <vector>
#include <string>

// Represents an individual tile in the tileset.
struct TileData
{
	// The texture for this tile.
	std::string texture;

	// Image rect of this tile, a 64x64 space.
	SDL_Rect rect = { 0, 0, 64, 64 };

	// The collision rectangles for this Tile.
	std::vector < SDL_FRect > collisionRects;
};

// Represents an internal tileset.
struct Tileset
{
	// The image of our tileset.
	std::string texture;

	// Our list of tiles.
	std::vector < TileData > tiles;

	// Releases all of our tile textures.
	void FreeTiles();

	TileData GetTile(int tileID) { return tiles[tileID]; }
};

#endif