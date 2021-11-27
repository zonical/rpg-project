# Entities
There are two types of entities, **global** and **local/level**. **Global entities** will always persist in most game states (minus the Main Menu where everything will be destroyed). They are completely hardcoded and are usually static in function. **Local/level** entities are also defined in code but are very abstract. JSON script files should fill most information (e.g for an NPC, the sprite and dialogue). They are stored in level information and are created and destroyed when it's host level is created or destroyed. 

The base Entity class, `Entity`, inherits `Taggable`, `Renderable`, and `Inputtable`. It can have tags assigned to it, be rendered to the screen and receive inputs from SDL Events. Most functions are virtual and left blank so programmers can easily override them. Programmers should inherit from Entity and inherit only needed functions.

### Definition Example
*(character.h)*
```c++
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
```

## Rendering
Rendering is done through the `Draw` function. Textures will be directly drawn to the Renderer using `SDL_RenderCopyF`.

The absolute position of entities will be offset by the `camera` FRect in `GameEngine->gRenderer`. The `Character` entity is responsible for offsetting the camera since it is the representation of the Player.

### Rendering Example
*(character.cpp)*
```c++
void Character::Draw(SDL_Window* win, SDL_Renderer* ren)
{
    // Render our character.
    SDL_RenderCopyF(ren, this->texture, NULL, &destinationRect);
    return;
}
```

## Positioning
There are two different types of positioning values for each entity. The **absolute position** is controlled by the `destinationRect` and it's where the entity is positioned on the window. The *x* and *y* values will be offset by the camera. The **level** position is similar to the absolute position with the exception that it's not offset by the camera.