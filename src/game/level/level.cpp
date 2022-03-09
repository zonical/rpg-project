#include "SDL/SDL.h"
#include "json/json.hpp"
#include "rpg/engine.h"
#include "rpg/entities/npc.h"
#include "rpg/entities/door.h"
#include "rpg/entities/light.h"
#include "rpg/level/level.h"
#include "rpg/level/tile.h"
#include "rpg/resources/tilesetmanager.h"
#include "rpg/states/overworld.h"
#include <memory>
#include <thread>

using json = nlohmann::json;

Level::Level()
{
}

Level::~Level()
{
	// Unload this level. Destroy all of our entities
	// and destroy all of our tiles.
	FreeResources();

	OverworldState::instance()->OnLevelShutdown();
}

// Grabs our character from our entity list.
Character* Level::GetCharacter()
{
	for (auto& entityLayer : lEntities)
	{
		for (auto& entity : entityLayer)
		{
			if (entity->HasTag("Character"))
			{
				return dynamic_cast<Character*>(entity.get());
			}
		}
	}
	return nullptr;
}

void Level::FreeResources()
{
	// Free all of our things.
	for (auto& layer : lTiles)
	{
		for (auto& tile : layer)
		{
			Tile *ptr = tile.release();
			delete ptr;
		}
	}

	for (auto& layer : lEntities)
	{
		for (auto& entity : layer)
		{
			Entity* ptr = entity.release();
			delete ptr;
		}
	}
}

// Loads a level and populates it.
bool Level::LoadLevel(std::string levelPath)
{
	// Load our level.
	auto levelData = GameEngine->LoadJSON(levelPath);

	// Create our entities.
	CreateEntities(levelData);

	// Construct our tileset.
	CreateTiles(levelData);

	// Create our collision.
	CreateCollision(levelData);
	
	std::cout << "[LEVEL] Created level " << levelPath << std::endl;
	OverworldState::instance()->OnLevelLoaded();

	return true;
}

// Loads a level and creates tiles.
bool Level::CreateTiles(json levelData)
{
	try
	{
		// Set the renderer background color.
		auto backgroundHexColor = levelData["backgroundcolor"].get<std::string>();
		sscanf_s(backgroundHexColor.c_str(), "#%02x%02x%02x", 
			&EngineResources.backgroundColor.r, 
			&EngineResources.backgroundColor.g, 
			&EngineResources.backgroundColor.b);
		EngineResources.backgroundColor.a = 255;

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

		// Fix up the source of the image.
		tilesetSource = CleanupSourceImage(tilesetSource);

		// Grab our tileset.
		Tileset tileset = EngineResources.tilesets.GetTileset(tilesetSource);

		// Grab our layers and iterate over all of them.
		auto levelLayers = levelData["layers"].get<std::vector<json>>();
		int layerCount = 0;

		// Grab all of our lights for lighting purposes.
		std::vector<Light*> lights;

		for (int l = 0; l < MAX_TILE_LAYERS; l++)
		{
			for (auto& entity : lEntities[l])
			{
				// Only deal with lights.
				if (!entity->HasTag("Light")) continue;

				// Convert entity to Light.
				Light* light = dynamic_cast<Light*>(entity.get());

				// Store it for later.
				lights.push_back(light);
			}
		}

		// Iterate over our vector and grab our data.
		for (auto& layer : levelLayers)
		{
			// Check the type of this layer. If it's not a tile layer then
			// we can just skip it.
			auto type = layer["type"].get<std::string>();
			if (type != "tilelayer") continue;

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
					float levelX = layerXOffset + 64 * x;
					float levelY = layerYOffset + 64 * y;

					// Grab our TileData via this tiles ID.
					int tileID = tiles[tileCount];

					// Grab the internally stored TileData.
					TileData tileData = tileset.GetTile(tileID);
					
					// If we have any collision data for this tile, create a new CollisionRect object
					// and store it in our level collision list.
					for (auto& tileCollisionObj : tileData.collisionRects)
					{
						CollisionRect collision;
						collision.collisionRect = tileCollisionObj;

						// These are offset from the topleft of the tile.
						collision.levelX = levelX + tileCollisionObj.x;
						collision.levelY = levelY + tileCollisionObj.y;

						// Insert into our list.
						lCollisionR.push_back(collision);
;					}

					// Construct a tile.
					std::unique_ptr<Tile> tile(new Tile(levelX, levelY, tileData));

					// Create our lighting for our tile.
					// Loop over all of our lights and calculate lighting for this tile.
					for (auto& light : lights)
					{
						SDL_Color lightColor = CalculateLightingFromEntityToTile(light, tile.get(), light->colorModifier, light->intensity);

						tile->colorModifier.r = (int)std::min(tile->colorModifier.r + lightColor.r, 255);
						tile->colorModifier.g = (int)std::min(tile->colorModifier.g + lightColor.g, 255);
						tile->colorModifier.b = (int)std::min(tile->colorModifier.b + lightColor.b, 255);

						//printf("Tile: %d, RGB: %d, %d, %d, Light RGB: %d, %d, %d, %d\n",
						//	tileCount, tile->colorModifier.r, tile->colorModifier.g, tile->colorModifier.b,
						//	light->colorModifier.r, light->colorModifier.g, light->colorModifier.b, light->intensity);

					}

					// Put our tile in our level list.
					lTiles[layerCount].push_back(std::move(tile));

					// Increment the tiles that we've gone through.
					tileCount++;
				}
			}

			// Increment our layer count so we can store this.
			layerCount++;
		}
		return true;
	}
	catch (std::exception& Exception)
	{
		std::cout << "[LEVELS] Failed to load level tiles." << " : " << Exception.what() << std::endl;
		return false;
	}
	return false;
}

// Loads a level and creates collision.
bool Level::CreateCollision(json levelData)
{
	try
	{
		// Grab our layers and iterate over all of them.
		auto levelLayers = levelData["layers"].get<std::vector<json>>();
		int layerCount = 0;

		// Iterate over our vector and grab our data.
		for (auto& layer : levelLayers)
		{
			// Check the type of this layer. If it's not an object layer then
			// we can just skip it.
			auto type = layer["type"].get<std::string>();
			if (type != "objectgroup") continue;

			// Grab our objects.
			auto objects = layer["objects"].get<std::vector<json>>();
			for (auto& object : objects)
			{
				// Construct special entities depending on what type they are.
				auto type = object["type"].get<std::string>();

				if (type != "collision_rect") continue;
				
				// Construct our rectangle.
				CollisionRect collision;
				collision.levelX = object["x"].get<float>();
				collision.levelY = object["y"].get<float>();
				collision.collisionRect.w = object["width"].get<float>();
				collision.collisionRect.h = object["height"].get<float>();

				// Put it in our list of collision objects.
				lCollisionR.push_back(collision);
			}
			layerCount++;
		}
		return true;
	}
	catch (std::exception& Exception)
	{
		std::cout << "[LEVELS] Failed to load level collision: " << Exception.what() << std::endl;
		return false;
	}
	return false;
}

// Loads a level and creates entities.
bool Level::CreateEntities(json levelData)
{
	try
	{
		// Grab our layers and iterate over all of them.
		auto levelLayers = levelData["layers"].get<std::vector<json>>();
		int layerCount = 0;

		// Iterate over our vector and grab our data.
		for (auto& layer : levelLayers)
		{
			// Check the type of this layer. If it's not an object layer then
			// we can just skip it.
			auto type = layer["type"].get<std::string>();
			if (type != "objectgroup") continue;

			// Grab our objects.
			auto objects = layer["objects"].get<std::vector<json>>();
			for (auto& object : objects)
			{
				// Construct special entities depending on what type they are.
				auto type = object["type"].get<std::string>();

				// Construct our entity.
				std::unique_ptr<Entity> entity;

				// Object is a character.
				if (type == "character")
				{
					// If we already have a Character entity, ignore it.
					if (GetCharacter() != nullptr) continue;

					// Create a new Character entity.
					std::unique_ptr<Entity> ptr(new Character());
					entity.swap(ptr);
				}

				// Object is an NPC.
				else if (type == "npc")
				{
					// Start constructing our NPC.
					std::unique_ptr<Entity> ptr = std::make_unique<NPCEntity>();
					entity.swap(ptr);
				}

				// Object is a door.
				else if (type == "door")
				{
					// Start constructing our door.
					std::unique_ptr<Entity> ptr = std::make_unique<DoorEntity>();
					entity.swap(ptr);
				}
				else if (type == "landmark")
				{
					// As this is a literal point on a map, we're not going to bother
					// with creating the nessacary overhead for this entity. Instead, we'll
					// just create a base entity instead and fill it's information with that.
					std::unique_ptr<Entity> ptr = std::make_unique<Entity>();
					entity.swap(ptr);
				}
				// Object is a light.
				else if (type == "light")
				{
					// Start constructing our door.
					std::unique_ptr<Entity> ptr = std::make_unique<Light>();
					entity.swap(ptr);
				}

				// This isn't an entity we recognize.
				else continue;

				// Set basic properties about our entity.
				entity->classname = type;
				entity->targetname = object["name"].get<std::string>();
				entity->levelX = object["x"].get<float>();
				entity->levelY = object["y"].get<float>();
				entity->destinationRect.w = object["width"].get<float>();
				entity->destinationRect.h = object["height"].get<float>();

				// Do we have any special properties?
				if (object.contains("properties")) entity->tiledProperties = object["properties"].get<std::vector<json>>();

				entity->OnEntityCreated();

				// Push our final entity to the list of entities.
				lEntities[layerCount].push_back(std::move(entity));
				
			}
			layerCount++;
		}
		return true;
	}
	catch (std::exception& Exception)
	{
		std::cout << "[LEVEL] Failed to load level entities: " << Exception.what() << std::endl;
		return false;
	}
	return false;
}

std::string Level::CleanupSourceImage(std::string string)
{
	int stringIndex;

	// Also remove the .. in the front.
	stringIndex = 0;
	while ((stringIndex = string.find("..")) != std::string::npos)
	{
		string.erase(stringIndex, std::string("..").length());
	}

	// Add "assets" in the front.
	string = "assets" + string;
	return string;
}

void Level::LevelUpdate(float dT)
{
	// For all of our entities, check collisions between them and
	// collision rectangles and stuff.
	
	for (auto& entityLayer : lEntities)
	{
		for (auto& entity : entityLayer)
		{
			// Update all of our entities.
			entity->Update(dT);
		}
	}
}