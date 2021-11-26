#ifndef HEADER_H_ENTITY
#define HEADER_H_ENTITY
#include "SDL/SDL.h"
#include "rpg/tags.h"

#include <vector>
#include <algorithm>

class Entity : public Taggable
{
public:
    Entity();
    ~Entity();

    virtual void OnEntitySpawned() {};
    virtual void OnEntityDestroyed() {};

    // This is the main rendering function for everything related to this entity.
    virtual void DrawEntity(SDL_Window*, SDL_Renderer*) {};

    // Updating and logic.
    virtual void Update(float deltaTime) {};
    float nextUpdate;   // If there is to be a delay in processing the logic in the
                        // Think function, have an already defined variable for it.

    virtual void OnKeyboardInput(SDL_Keycode keyCode, bool pressed, bool released, bool repeat) {};
    virtual void OnMouseMotion(int x, int y, int xrel, int yrel, Uint32 state) {};
    virtual void OnMouseButtonPressed(bool pressed, bool released, int button, int clicks, int x, int y) {};
    virtual void OnMouseWheelScrolled(int x, int y, int direction) {};

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

    // Positioning.

    // These x and y values are the literal X and Y values in the SDL space.
    // These will get manipulated by things like the camera.
    virtual float x() { return destinationRect.x; }
    virtual float y() { return destinationRect.y; }

    virtual float originX() { return destinationRect.x + (destinationRect.w / 2); }
    virtual float originY() { return destinationRect.y + (destinationRect.h / 2); }

    // This is a float-rectangle used for literal positioning of this entity.
    SDL_FRect    destinationRect;

    // This is local-world positioning. These values aren't changed by camera shifting.
    float cameraXOffset, cameraYOffset;

    // Size value grabbers.
    virtual float w() { return destinationRect.w; }
    virtual float h() { return destinationRect.h; }
};

#endif