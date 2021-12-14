#pragma once
#ifndef DOOR_H
#define DOOR_H
#include "rpg/entity.h"
#include <json/json.hpp>
#include <vector>
#include <string>
#include <memory>

using json = nlohmann::json;

// The class that represents an NPC.
class DoorEntity : public Entity
{
public:
	DoorEntity() {};
	~DoorEntity() {};

	bool enabled;
	std::string landmarkName;
	std::string levelDestination;
	
	bool alreadyFaded;

	// Update logic.
	void Update(float dT);
	void OnEntitySpawned();
};
#endif