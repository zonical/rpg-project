#ifndef HEADER_H_ENTITY
#define HEADER_H_ENTITY
#include "SDL/SDL.h"
#include "rpg/base/taggable.h"
#include "rpg/base/renderable.h"
#include "rpg/base/inputtable.h"
#include "json/json.hpp"

#include <string>
#include <vector>
#include <algorithm>

class Entity 
    : public Taggable, public Renderable, public Inputtable
{
public:
    Entity();
    virtual ~Entity() {};
    std::string classname;
    std::string targetname;

    virtual void OnEntityCreated() {};
    virtual void OnEntitySpawned() {};
    virtual void OnEntityDestroyed() {};

    // Updating and logic.
    virtual void Update(float deltaTime) {};
    float nextUpdate;   // If there is to be a delay in processing the logic in the
                        // Think function, have an already defined variable for it.

    // Performing a "use" action.
    Entity*         useActivator = nullptr; // The Entity that uses ourselves.
    bool            isCurrentlyUsed = false;
    float           useDistance = 0.0f; // Distance the Character needs to be away to trigger OnUse().
    virtual void    OnUse(Entity* activator) {};
    virtual void    OnUseFinished(Entity* activator) {};

    // Color modifier for this entity - This is primarily used for lighting.
    SDL_Color colorModifier;

    // Custom properties from Tiled.
    using json = nlohmann::json;
    std::vector<json> tiledProperties;

    // Check for collision between another entity.
    virtual bool CheckCollision(Entity *ent)
    {
        // If this entity is something we can't actually collide with,
        // don't bother checking it.
        if (!ent->HasTag(Tag_Collision)) return true;

        // %s1 = us
        // %s2 = other
        float left1, left2;
        float right1, right2;
        float top1, top2;
        float bottom1, bottom2;

        left1 = this->levelX;
        right1 = this->levelX + this->w();
        top1 = this->levelY;
        bottom1 = this->levelY + this->h();

        left2 = ent->levelX;
        right2 = ent->levelX + ent->w();
        top2 = ent->levelY;
        bottom2 = ent->levelY + ent->h();

        if      ( bottom1 <= top2 ) return false;
        if      ( top1 >= bottom2 ) return false;
        if      ( right1 <= left2 ) return false;
        if      ( left1 >= right2 ) return false;

        // We're not colliding with anything.
        return true;
    }

    virtual bool CheckCollision(SDL_FRect rect)
    {
        // %s1 = us
        // %s2 = other
        float left1, left2;
        float right1, right2;
        float top1, top2;
        float bottom1, bottom2;

        left1 = this->x();
        right1 = this->x() + this->w();
        top1 = this->y();
        bottom1 = this->y() + this->h();

        left2 = rect.x;
        right2 = rect.x + rect.w;
        top2 = rect.y;
        bottom2 = rect.y + rect.h;

        if (bottom1 <= top2) return false;
        if (top1 >= bottom2) return false;
        if (right1 <= left2) return false;
        if (left1 >= right2) return false;

        // We're not colliding with anything.
        return true;
    }
    bool isColliding;
};

#endif