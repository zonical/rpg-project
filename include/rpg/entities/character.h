#ifndef CHAR_H_ENTITY
#define CHAR_H_ENTITY

#include "rpg/entity.h"
#include "SDL/SDL.h"
#include <memory>

class Character : public Entity
{
public:
    // Initalizer.
    Character();
    ~Character() {};

    void OnEntitySpawned();

    // Rendering.
    void Draw(SDL_Window*, SDL_Renderer*);
    // Inputs.
    void OnKeyboardInput(SDL_Keycode keyCode, bool pressed, bool released, bool repeat);

    float dT;

    // Update logic.
    void Update(float);
    void HandleMovement();
    void HandleUsing();
    void Move(float x, float y);
    void ForcePosition(float x, float y);
    void ResetAllMovement();

private:
    int lastValidDirection = 0;
    float lastValidX;
    float lastValidY;

    std::shared_ptr<SDL_Texture> texture;

    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool use = false;
    
    SDL_FRect collisionRect;
};

#endif