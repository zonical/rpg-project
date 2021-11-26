#include "rpg/entity.h"
#include "SDL/SDL.h"
#ifndef CHAR_H_ENTITY
#define CHAR_H_ENTITY

class Character : public Entity
{
public:
    // Initalizer.
    Character();

    ~Character()
    {
        GameEngine->RemoveEntity(this);
    }

    void OnEntitySpawned();

    // Rendering.
    void DrawEntity(SDL_Window*, SDL_Renderer*);

    // Inputs.
    void OnKeyboardInput(SDL_Keycode keyCode, bool pressed, bool released, bool repeat);

    float dT;

    // Update logic.
    void Update(float);
    void HandleMovement();
    void HandleCamera();

private:
    SDL_Rect char_main_image_rect; 
    SDL_Texture* texture;

    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
};

#endif