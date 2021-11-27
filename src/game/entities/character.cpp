#include "SDL/SDL.h"
#include "rpg/renderer.h"
#include "rpg/engine.h"
#include "rpg/entities/character.h"

Character::Character() : Entity()
{
    // Set the rect sizes and positions.
    levelX = DEFAULT_SCREEN_WIDTH / 2;
    levelY = DEFAULT_SCREEN_HEIGHT / 2;
    destinationRect.w = 64;
    destinationRect.h = 64;

    // This entity can collide with other entities.
    this->AddTag(Tag_Renderable);
    this->AddTag(Tag_Collision);

    texture = GameEngine->gRenderer.GetTexture("assets/sprites/debug/test_cube.png");
}

void Character::OnEntitySpawned()
{
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
        printf("released\n");
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
}

void Character::HandleMovement()
{
    bool colliding = false;
    float speed = 0.25 * dT;

    // If we're colliding something, negate our speed so we move backwards.
    if (colliding) speed = 0;

    // Move ourselves if we're pressing a movement key.
    if (up)
    {
        this->levelY -= speed;
        GameEngine->gRenderer.camera.y -= speed;
    }

    if (down)
    {
        this->levelY += speed;
        GameEngine->gRenderer.camera.y += speed;
    }

    if (left)
    {
        this->levelX -= speed;
        GameEngine->gRenderer.camera.x -= speed;
    }

    if (right)
    {
        this->levelX += speed;
        GameEngine->gRenderer.camera.x += speed;
    }
}


void Character::Draw(SDL_Window* win, SDL_Renderer* ren)
{
    // Render our character.
    SDL_RenderCopyF(ren, this->texture, NULL, &destinationRect);
    return;
}