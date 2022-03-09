#pragma once

// Define Python.h here before we include anything from the standard library.
// This is recommended by the official documentation: https://docs.python.org/3/extending/extending.html
#define PY_SSIZE_T_CLEAN		/* Make "s#" use Py_ssize_t rather than int. (https://docs.python.org/3/extending/extending.html#parsetuple) */
#include <Python.h>

// Core engine files.
#include "rpg/engine.h"						// Defines the engine class.
#include "rpg/gui/base.h"					// Base class for GUI elements and a GUI object for states.
#include "rpg/entity.h"						// Base class for entities.
#include "rpg/gamestate.h"					// Base class for gamestates.
#include "rpg/base/inputtable.h"			// Base class that adds inputtable functionality.
#include "rpg/base/renderable.h"			// Base class for renderable SDL objects.
#include "rpg/base/taggable.h"				// Base class that includes unique-tagging functions.
#include "rpg/level/level.h"				// Level class.
#include "rpg/level/tile.h"					// Tile class.
#include "rpg/level/tileset.h"				// Tileset class.
#include "rpg/resources.h"					// Resource class.
#include "rpg/resources/dialoguemanager.h"	// Dialogue manager resource.
#include "rpg/resources/fontmanager.h"		// Font manager resource.
#include "rpg/resources/texturemanager.h"	// Texture manager resource.
#include "rpg/resources/tilesetmanager.h"	// Tileset manage resource.
#include "rpg/entities/character.h"			// Character entity.

// SDL.
#include "SDL/SDL.h"						// Core SDL.
#include "SDL/SDL_ttf.h"					// SDL_ttf library.
#include "SDL/SDL_image.h"					// SDL_image library.

// Standard library stuff.
#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <cmath>
#include <map>
#include <filesystem>
#include <iterator>
#include <algorithm>
#include <array>
#include <functional>
#include <vector>
#include <list>

// Other external includes.
#include "json/json.hpp"					// Header-only JSON library from https://github.com/nlohmann/json

// Definitions.
#define MAX_GUI_LAYERS			16
#define MAX_TILE_LAYERS			16
#define LEVEL_TRANSITION_FADE	25