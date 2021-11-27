#ifndef CHAR_H_ENTITY
#define CHAR_H_ENTITY

#include "rpg/entity.h"
#include "SDL/SDL.h"

class Character : public Entity
{
public:
    // Initalizer.
    Character();

    void OnEntitySpawned();

    // Rendering.
    void Draw(SDL_Window*, SDL_Renderer*);
    // Inputs.
    void OnKeyboardInput(SDL_Keycode keyCode, bool pressed, bool released, bool repeat);

    float dT;

    // Update logic.
    void Update(float);
    void HandleMovement();

private:
    SDL_Texture* texture;

    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
};

#endif