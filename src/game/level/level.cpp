#include "SDL/SDL.h"
#include "json/json.hpp"
#include "rpg/engine.h"
#include "rpg/level/level.h"
#include "rpg/level/tile.h"
#include "rpg/resources/tilesetmanager.h"

using json = nlohmann::json;

Level::Level()
{
}

Level::~Level()
{
	// Unload this level. Destroy all of our entities
	// and destroy all of our tiles.
	FreeResources();
}

void Level::FreeResources()
{
	// Free all of our entities.
	for (auto& entity : lEntities)
	{
		delete entity;
	}
	lEntities.clear();

	// Free all of our tiles.
	for (auto& tileLayer : lTiles)
	{
		for (auto& tile : tileLayer)
		{
			delete tile;
		}
		tileLayer.clear();
	}
}

// Loads a level and populates it.
bool Level::LoadLevel(std::string levelPath)
{
	try
	{
		// Load the file. It should be stored in JSON.
		auto levelData = GameEngine->LoadJSON(levelPath);

		// The main editing software that we can use for tilesets is called Tiled.
		// We do have some constant rules though.
		//		1) ALL tiles and tilesets must be 64x64
		//		2) Tiles are read from left to right and then up to down.
		//		3) Rotation of tiles are currently not supported. (29/11/21)
		// Tiled is nice and it gives us the width and height of each level, so we
		// can use that to our advantage.

		// Grab our tileset.
		auto levelTilesets = levelData["tilesets"].get<std::vector<json>>();

		// For now, we're going to limit ourselves to using one tileset at a time. In the future,
		// it could be possible to add support for multiple tilesets at once.
		auto tilesetJSON = levelTilesets[0];

		// Grab the source file of the tileset.
		auto tilesetSource = tilesetJSON["source"].get<std::string>();

		
		// Quickly fix our tileset path by removing some unneeded slashes and adding "assets/tiles"
		// to the front of the path.
		int stringIndex;
		while ((stringIndex = tilesetSource.find("\\/")) != std::string::npos)
		{
			tilesetSource.replace(stringIndex, std::string("/").length(), std::string("/"));
		}

		// Also remove the .. in the front.
		stringIndex = 0;
		while ((stringIndex = tilesetSource.find("..")) != std::string::npos)
		{
			tilesetSource.erase(stringIndex, std::string("..").length());
		}

		// Add "assets" in the front.
		tilesetSource = "assets" + tilesetSource;

		// Grab our tileset.
		TilesetManager test = EngineResources.tilesets;
		Tileset tileset = EngineResources.tilesets.GetTileset(tilesetSource);

		// Grab our layers and iterate over all of them.
		auto levelLayers = levelData["layers"].get<std::vector<json>>();
		int layerCount = 0;

		// Iterate over our vector and grab our data.
		for (auto& layer : levelLayers)
		{
			// Get the height and width, this determines our x and y positioning of the tiles.
			auto layerHeight = layer["height"].get<int>();
			auto layerWidth = layer["width"].get<int>();

			// Get the x and y offsets of this layer.
			auto layerXOffset = layer["x"].get<int>();
			auto layerYOffset = layer["y"].get<int>();

			// Get our tile data. This is stored in an array of ID's which is not split so
			// we'll do it ourselves.
			auto tiles = layer["data"].get<std::vector<int>>();

			int tileCount = 0;

			for (int y = 0; y < layerHeight; y++)
			{
				for (int x = 0; x < layerWidth; x++)
				{
					// Grab our tileID.
					int tileID = tiles[tileCount] - 1;  // A -1 is added here as internally our tiles start at 0
														// but Tiled has them start at 1.

					// Grab the internally stored TileData.
					TileData tileData = tileset.GetTile(tileID);

					// Construct a tile.
					Tile* tile = new Tile(layerXOffset + 64 * x, layerYOffset + 64 * y, tileData);

					// Put our tile in our level list.
					lTiles[layerCount].push_back(tile);

					// Increment the tiles that we've gone through.
					tileCount++;
				}
			}

			// Increment our layer count so we can store this.
			layerCount++;
		}
		printf("[LEVELS] Loaded level %s tiles\n", levelPath.c_str());
		return true;
	}
	catch (std::exception& Exception)
	{
		printf("[LEVELS] Failed to load level %s: %s\n", levelPath.c_str(), Exception.what());
	}
}
