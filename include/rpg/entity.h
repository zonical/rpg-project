#ifndef HEADER_H_ENTITY
#define HEADER_H_ENTITY
#include "SDL/SDL.h"
#include "rpg/taggable.h"
#include "rpg/renderable.h"
#include "rpg/inputtable.h"

#include <vector>
#include <algorithm>

class Entity 
    : public Taggable, public Renderable, public Inputtable
{
public:
    Entity();

    virtual void OnEntitySpawned() {};
    virtual void OnEntityDestroyed() {};

    // Updating and logic.
    virtual void Update(float deltaTime) {};
    float nextUpdate;   // If there is to be a delay in processing the logic in the
                        // Think function, have an already defined variable for it.

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

        left1 = this->x();
        right1 = this->x() + this->w();
        top1 = this->y();
        bottom1 = this->y() + this->h();

        left2 = ent->x();
        right2 = ent->x() + ent->w();
        top2 = ent->y();
        bottom2 = ent->y() + ent->h();

        if      ( bottom1 <= top2 ) return false;
        if      ( top1 >= bottom2 ) return false;
        if      ( right1 <= left2 ) return false;
        if      ( left1 >= right2 ) return false;

        // We're not colliding with anything.
        return true;
    }
};

#endif