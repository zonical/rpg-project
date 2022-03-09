#include "SDL/SDL.h"
#include "rpg/engine.h"
#include "rpg/level/tile.h"

#include "rpg/states/overworld.h"
#include "rpg/gamestate.h"

#include <algorithm>

// Base constructor, just adds a tag saying we should
// render this thing.
Tile::Tile()
{
	this->AddTag(Tag_Renderable);
}

// The actual constructor. Sets some positioning stuff in our rect.
Tile::Tile(float x, float y, TileData data)
{
	destinationRect.w = 64;
	destinationRect.h = 64;

	// Set the position of the destionation rects.
	levelX = x;
	levelY = y;

	// And now, the x and y position of our rect.
	imageRect.x = data.rect.x;
	imageRect.y = data.rect.y;
	imageRect.w = data.rect.w;
	imageRect.h = data.rect.h;

	this->data = data;
	this->texture = std::shared_ptr<SDL_Texture>(EngineResources.textures.GetTexture(data.texture));
}

// Render this tile to the screen.
void Tile::Draw(SDL_Window* win, SDL_Renderer* ren)
{
	Renderable::Draw(win, ren);
	SDL_RenderCopyF(ren, texture.get(), &imageRect, &renderedRectangle);
}