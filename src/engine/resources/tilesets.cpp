#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "rpg/engine.h"
#include "rpg/resources/tilesetmanager.h"
#include <string>
#include <map>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

void TilesetManager::Initalize()
{
    std::string path = "assets/tiles";
    for (const auto& entry : fs::recursive_directory_iterator(path))
    {
        // Get the path as a string.
        std::string path = entry.path().string();

        // Replace the slashes in our path with forward slashes.
        int stringIndex;
        while ((stringIndex = path.find("\\")) != std::string::npos)
        {
            path.replace(stringIndex, std::string("/").length(), std::string("/"));
        }

        // We only load PNG files. This check is here as folders count under the iterator
        // and we obviously can't load a folder.
        if (path.ends_with(".png")) LoadTileset(path);
    }
}

Tileset TilesetManager::GetTileset(std::string tilesetPath)
{
    // Double check to see if this tileset exists.
    if (!TilesetExists(tilesetPath)) return {};

    // Find our tileset with the tileset path.
    auto iter = tilesets.find(tilesetPath);
    return iter->second;
}

bool TilesetManager::TilesetExists(std::string tilesetPath)
{
    // Find our tileset with the tileset path.
    auto iter = tilesets.find(tilesetPath);
    return !(iter == tilesets.end());
}

bool TilesetManager::RemoveTileset(std::string tilesetPath)
{
    // Double check to see if this tileset exists.
    if (!TilesetExists(tilesetPath)) return false;

    // Get the tileset from our map of tilesets.
    Tileset tileset = GetTileset(tilesetPath);
    
    tileset.FreeTiles();

    // Remove this tileset from the map.
    tilesets.erase(tilesetPath);
    return true;
}

void TilesetManager::ReleaseAllTilesets()
{
    // Loop through all of our tilesets and release them.
    for (std::map<std::string, Tileset>::iterator iter = tilesets.begin();
        iter != tilesets.end(); iter++)
    {
        iter->second.FreeTiles();
    }
}

bool TilesetManager::LoadTileset(std::string tilesetPath)
{
    if (EngineResources.renderer == NULL)
    {
        // Stop.
        printf("[ERROR] Could not load tileset for tileset %s.\n", tilesetPath.c_str());
        return false;
    }

    // Create a tileset from the surface.
    SDL_tileset* tileset = IMG_Loadtileset(EngineResources.renderer, tilesetPath.c_str());
    if (tileset == NULL)
    {
        printf("[ERROR] Could not create tileset for tileset %s: %s.\n", tilesetPath.c_str(), IMG_GetError());
        return false;
    }

    // Get the width and height of the original tileset.
    int w, h;
    SDL_Querytileset(tileset, NULL, NULL, &w, &h);

    // If this tileset already exists in our map, remove it first, and then
    // put in this new tileset.
    if (TilesetExists(tilesetPath))
    {
        // Remove it.
        RemoveTileset(tilesetPath);
    }

    // Start constructing our tileset.
    Tileset tileset;
    tileset.tilesetTiles = tileset;

    // Construct our tileset. The size for each tile in our tileset is a 64x64 rectangle.
    // We read strictly from left to right and then top to bottom.
    int yIterations = floor(h / 64);
    int xIterations = floor(w / 64);

    for (int y = 0; y < yIterations; y++)
    {
        for (int x = 0; x < xIterations; x++)
        {
            // Construct a tile.
            TileData tile;
            tile.rect.x = 64 * x;
            tile.rect.y = 64 * y;
            tile.tileTexture = tileset;

            // Add our tile ot the tileset.
            tileset.tiles.push_back(tile);
        }
    }

    // Store this new tileset in our map.
    tilesets.insert(std::pair<std::string, Tileset>(tilesetPath, tileset));
    printf("[TILESETS] Tileset with %d tiles loaded: %s.\n", tileset.tiles.size(), tilesetPath.c_str());
    return true;
}