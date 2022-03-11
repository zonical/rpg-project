#include "rpg/rpg.h"
#include "rpg/entities/door.h"
#include "rpg/states/overworld.h"

void DoorEntity::OnEntitySpawned()
{
	// Loop through the doors properties and set stuff acoordingly.
	for (auto& element : tiledProperties)
	{
		// Grab the name of this element.
		auto name = element["name"].get < std::string >();

		// Set our properties.
		if (name == "level") levelDestination = element["value"].get< std::string >();
		if (name == "landmark_entity") landmarkName = element["value"].get< std::string >();
		if (name == "enabled") enabled = element["value"].get<bool>();
	}
}

void DoorEntity::Update(float dT)
{
	if (GameEngine->GetOverworldState()->gLevelTransData.transitionFlag) return;

	// Grab our character entity.
	Character* character = GameEngine->GetOverworldState()->gLevel->GetCharacter();

	// Check to see if the character is colliding with us. If they are,
	// perform our level transition.
	if (CheckCollision(character))
	{
		// Don't let the character move anymore.
		character->AddTag("DontMove");

		if (!alreadyFaded) EngineResources.FadeToBlack(LEVEL_TRANSITION_FADE); alreadyFaded = true;
		if (alreadyFaded && EngineResources.currentlyFading) return;

		// Set our level transition data in the engine so the engine knows what
		// to do when the next Character entity spawns in.
		GameEngine->GetOverworldState()->gLevelTransData.landmark_name = this->landmarkName;
		GameEngine->GetOverworldState()->gLevelTransData.new_level = this->levelDestination;
		GameEngine->GetOverworldState()->gLevelTransData.transitionFlag = true;

		// Exit, we'll be deleted soon.
		return;
	}
}