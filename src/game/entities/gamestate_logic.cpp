#include "SDL/SDL.h"

#include "rpg/engine.h"
#include "rpg/entities/gamestate_logic.h"
#include "rpg/entities/character.h"

GameStateLogic::GameStateLogic()
{
	// Initalize everything for this entity.
	// This is a logic entity, so we're not visible and we're not doing anything much
	// to do with keyboards and inputs and stuff. This just handles gameplay in its
	// update function.
	this->AddTag(Tag_NotRendering);
}

void GameStateLogic::OnEntitySpawned()
{
	// When we spawn into the level, we'll create any other nessacary entities that we need.
	// We'll create a Character object, and assign it to the Player as it's pawn (the thing
	// the player controls).
	Character* character = new Character();
	GameEngine->gPlayer.SetPlayerPawn(character);
}

void GameStateLogic::Update(float dT)
{
	// We're now going to process gameplay logic here depending on the current gamestate
	// stored in the global Engine object.
	
}