#include "SDL/SDL.h"

#include "rpg/engine.h"
#include "rpg/entities/gamestate_logic.h"
#include "rpg/entities/character.h"

GameStateLogic::GameStateLogic() {}

void GameStateLogic::InitalizeOverworld()
{
	// When we spawn into the level, we'll create any other nessacary entities that we need.
	// We'll create a Character object, and assign it to the Player as it's pawn (the thing
	// the player controls). This is only done if we don't already have one as this is considered
	// a global entity.
}

void GameStateLogic::Update(float dT)
{
	// We're now going to process gameplay logic here depending on the current gamestate
	// stored in the global Engine object.
	
}