#ifndef NPC_H
#define NPC_H
#pragma once
#include "rpg/entity.h"
#include "rpg/gui/textbox.h"
#include <json/json.hpp>
#include <vector>
#include <string>
#include <memory>

using json = nlohmann::json;

// The class that represents an NPC.
class NPCEntity : public Entity
{
public:
	NPCEntity();
	~NPCEntity();

	std::shared_ptr<SDL_Texture> npctexture;

	// Update logic.
	void Update(float dT);
	void OnUse(Entity* activator);
	void OnUseFinished(Entity* activator);

	void OnEntitySpawned();

	// The name of this NPC.
	std::string npcName;

	// What actions this NPC should do.
	json npcActions;
	
	// Using stuff.
	bool isCurrentlyActive = false;
	std::shared_ptr<Textbox> textbox;
	
	// Draw this NPC to the screen.
	void Draw(SDL_Window* win, SDL_Renderer* ren);
};
#endif