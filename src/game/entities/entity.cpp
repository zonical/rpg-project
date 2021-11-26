#include "rpg/engine.h"
#include "rpg/player.h"
#include "rpg/entity.h"

Entity::Entity()
{
    GameEngine->RegisterEntity(this);
    this->AddTag(Tag_Entity);
}

Entity::~Entity()
{
    GameEngine->RemoveEntity(this);
}

void Engine::RegisterEntity(Entity* ent)
{
    // Adds an entity to the global register..
    gEntities.push_back(ent);
    ent->OnEntitySpawned();
    printf("[ENTITY] Entity registered.\n");
}

void Engine::RemoveEntity(Entity* ent)
{
    // Removes an entity to the global register.
    auto it = std::find(gEntities.begin(), gEntities.end(), ent);
    if (it != gEntities.end())
    {
        // Remove it.
        gEntities.erase(it);
        it = gEntities.begin();

        printf("[ENTITY] Entity destroyed.\n");
    }
}