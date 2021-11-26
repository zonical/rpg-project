#include "SDL/SDL.h"
#include "rpg/renderer.h"
#include "rpg/engine.h"
#include "rpg/entities/character.h"

Character::Character() : Entity()
{
    // Set the rect sizes and positions.
    char_main_image_rect.w = 0;
    char_main_image_rect.h = 0;

    destinationRect.x = 300;
    destinationRect.y = 200;
    destinationRect.w = 64;
    destinationRect.h = 64;

    // This entity can collide with other entities.
    this->AddTag(Tag_Renderable);
    this->AddTag(Tag_Collision);
}

void Character::OnEntitySpawned()
{
    texture = GameEngine->gRenderer.GetTexture("assets/debug/test_cube.png");
}

void Character::OnKeyboardInput(SDL_Keycode keyCode, bool pressed, bool released, bool repeat)
{
    // If we're pressing or releasing a key and it's one of our movement keys, set it internally
    // so it can be handelled in our update function.
    // TODO: When this gets to the stage of when we want to add configs (e.g the user can bind keys),
    // add support for it here.

    // Are we allowed to move?
    if (!HasTag("Moveable")) return;

    if (pressed)
    {
        switch (keyCode)
        {
            case SDLK_w: up     = true; break;
            case SDLK_a: left   = true; break;
            case SDLK_s: down   = true; break;
            case SDLK_d: right  = true; break;
        }
    }

    if (released)
    {
        switch (keyCode)
        {
            case SDLK_w: up     = false; break;
            case SDLK_a: left   = false; break;
            case SDLK_s: down   = false; break;
            case SDLK_d: right  = false; break;
        }
    }
}

void Character::Update(float dT)
{
    this->dT = dT;

    HandleMovement();
    HandleCamera();
    
    //printf("Pos: wX: %f, wY: %f, lX: %f, lY: %f\n", x(), y(), cameraXOffset, cameraYOffset);
}

void Character::HandleMovement()
{
    bool colliding = false;
    float speed = 0.25 * dT;

    // If we're colliding something, negate our speed so we move backwards.
    if (colliding) speed = 0;

    // Move ourselves if we're pressing a movement key.
    if (up)         destinationRect.y -= speed;
    if (down)       destinationRect.y += speed;

    if (left)       destinationRect.x -= speed;
    if (right)      destinationRect.x += speed;
}

void Character::HandleCamera()
{
    bool colliding = false;
    float speed = 0.25 * dT;

    // If we're colliding something, negate our speed so we move backwards.
    if (colliding) speed = 0;

    // Offset values.
    float x = 0.0f;
    float y = 0.0f;

    if (up)         y = -speed;
    if (down)       y = speed;
    if (left)       x = -speed;
    if (right)      x = speed;

    // Move all entities by their literal position.
    for (auto& ent : GameEngine->gEntities)
    {
        ent->cameraXOffset += x;
        ent->cameraYOffset += y;
    }
}

void Character::DrawEntity(SDL_Window* win, SDL_Renderer* ren)
{
    // Render our character.
    SDL_RenderCopyF(ren, this->texture, NULL, &destinationRect);
    return;
}