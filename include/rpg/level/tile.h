#pragma once
#ifndef TILE_H
#define TILE_H
#include "SDL/SDL.h"
#include "rpg/base/renderable.h"

// Represents an individual tile on the level.
class Tile : public Renderable
{
private:
	// The texture for this tile.
	SDL_Texture* tileTexture;
public:
	// Set the texture of this tile.
	void SetTileTexture(int tileID);

	// Draws the tile to the screen.
	void Draw(SDL_Window* win, SDL_Renderer* ren);
};
#endif
