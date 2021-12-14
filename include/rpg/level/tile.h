#ifndef TILE_H
#define TILE_H

#define MAX_TILE_LAYERS 16

#include "SDL/SDL.h"
#include "rpg/base/renderable.h"
#include "rpg/base/taggable.h"
#include "rpg/level/tileset.h"
#include <memory>

// The visual representation of a tile.
class Tile : public Renderable, public Taggable
{
public:
	// Constructors for this tile.
	Tile();
	Tile(float x, float y, TileData data);

	std::shared_ptr<SDL_Texture> texture;
	TileData data;

	// Render the tile to the screen.
	void Draw(SDL_Window* win, SDL_Renderer* ren);
 };

#endif