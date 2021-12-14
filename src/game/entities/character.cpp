#include "SDL/SDL.h"
#include "rpg/resources.h"
#include "rpg/engine.h"
#include "rpg/entities/character.h"

Character::Character() : Entity()
{
    // This entity can collide with other entities.
    this->AddTag(Tag_Renderable);
    this->AddTag(Tag_Collision);
    this->AddTag("Character");

}

void Character::OnEntitySpawned()
{
    texture = EngineResources.textures.GetTexture("assets/sprites/the_man.png");

    // Set our position.
    levelX += -(this->w() / 2);
    levelY += -(this->h() / 2);

    // Set the rect sizes.
    destinationRect.w = 32;
    destinationRect.h = 64;

    collisionRect.w = 32;
    collisionRect.h = 32;

    // Set the position of our camera to be the X and Y position of our character minus
    // our camera offset. This is calculated by dividing the screen width and height
    // by two and subtracting the height and width of the character so it's perfectly centered.
    EngineResources.camera.x = levelX - (DEFAULT_SCREEN_WIDTH / 2) + (this->w() / 2);
    EngineResources.camera.y = levelY - (DEFAULT_SCREEN_HEIGHT / 2) + (this->h() / 2);
}

void Character::OnKeyboardInput(SDL_Keycode keyCode, bool pressed, bool released, bool repeat)
{
    // If we're pressing or releasing a key and it's one of our movement keys, set it internally
    // so it can be handelled in our update function.
    // TODO: When this gets to the stage of when we want to add configs (e.g the user can bind keys),
    // add support for it here.

    // Are we allowed to move?
    if (HasTag("DontMove"))
    {
        up = false;
        down = false;
        left = false;
        right = false;
        use = false;
        return;
    }

    if (pressed)
    {
        switch (keyCode)
        {
            case SDLK_w: up     = true; break;
            case SDLK_a: left   = true; break;
            case SDLK_s: down   = true; break;
            case SDLK_d: right  = true; break;
            case SDLK_q:
            {
                if (!repeat) use = true; 
                break;
            }
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
            case SDLK_q:
            {
                if (!repeat) use = false;
                break;
            }
        }
    }
}

void Character::Update(float dT)
{
    this->dT = dT;

    HandleMovement();

    if (use) HandleUsing();
}

void Character::Move(float x, float y)
{
    this->levelX += x;
    EngineResources.camera.x += x;

    this->levelY += y;
    EngineResources.camera.y += y;
}

void Character::ForcePosition(float x, float y)
{
    this->levelX = x;
    EngineResources.camera.x = levelX - (DEFAULT_SCREEN_WIDTH / 2) + (this->w() / 2);

    this->levelY = y;
    EngineResources.camera.y = levelY - (DEFAULT_SCREEN_HEIGHT / 2) + (this->h() / 2);
}

//sigh
bool SDL_FIntersectRect(SDL_FRect a, SDL_FRect b)
{

    // %s1 = us
    // %s2 = other
    float left1, left2;
    float right1, right2;
    float top1, top2;
    float bottom1, bottom2;

    left1 = a.x;
    right1 = a.x + a.w;
    top1 = a.y;
    bottom1 = a.y + a.h;

    left2 = b.x;
    right2 = b.x + b.w;
    top2 = b.y;
    bottom2 = b.y + b.h;

    if (bottom1 <= top2) return false;
    if (top1 >= bottom2) return false;
    if (right1 <= left2) return false;
    if (left1 >= right2) return false;

    // We're not colliding with anything.
    return true;
}

bool DoesAnythingIntersect(SDL_FRect testRect)
{
    if (GameEngine->gLevel == nullptr) return false;
    // Do collision detection with all of our collision objects in the level.
    for (auto& collisionRect : GameEngine->gLevel->lCollisionR)
    {
        if (SDL_FIntersectRect(collisionRect.collisionRect, testRect))
        {
            return true;
        }
    }
    return false;
}

void Character::HandleMovement()
{
    // Set the position of our collison rect.
    collisionRect.x = destinationRect.x;
    collisionRect.y = destinationRect.y + (destinationRect.h / 4);

    // If we are moving...
    if (up || down || left || right)
    {
        float baseSpeed = 5;
        float upSpeed = baseSpeed;
        float downSpeed = baseSpeed;
        float leftSpeed = baseSpeed;
        float rightSpeed = baseSpeed;

        if (up)
        {
            // Set where our collision rect will be.
            SDL_FRect testRect = collisionRect;
            testRect.y += -6;

            // Test if we're colliding with anything in the level.
            if (!DoesAnythingIntersect(testRect))
            {
                Move(0, -upSpeed);
            }
        }

        if (down)
        {
            // Set where our collision rect will be.
            SDL_FRect testRect = collisionRect;
            testRect.y += 6;

            // Test if we're colliding with anything in the level.
            if (!DoesAnythingIntersect(testRect))
            {
                Move(0, downSpeed);
            }
        }
        if (left)
        {
            // Set where our collision rect will be.
            SDL_FRect testRect = collisionRect;
            testRect.x += -6;

            // Test if we're colliding with anything in the level.
            if (!DoesAnythingIntersect(testRect))
            {
                Move(-leftSpeed, 0);
            }
        }
        if (right)
        {
            // Set where our collision rect will be.
            SDL_FRect testRect = collisionRect;
            testRect.x += 6;

            // Test if we're colliding with anything in the level.
            if (!DoesAnythingIntersect(testRect))
            {
                Move(rightSpeed, 0);
            }
        }
    }
}

void Character::HandleUsing()
{
    // Check to see our distance between all of our current level entities.
    // If we're within range, trigger the first entity and perform OnUse().
    if (GameEngine->gLevel == nullptr) return;

    // If we're currently using another entity, don't process another Use
    // event until we're out of it.
    if (this->isCurrentlyUsed) return;

    for (auto& entLayer : GameEngine->gLevel->lEntities)
    {
        for (auto& ent : entLayer)
        {
            // If this is us, what the heck are we doing?
            if (ent.get() == this) continue;

            // Calculate distance.
            float x = this->originX() - ent->originX();
            float y = this->originY() - ent->originY();
            float dist = sqrt(pow(x, 2) + pow(y, 2));

            // Can we use this ent?
            if (dist <= ent->useDistance)
            {
                // Perform our use ability.
                ent->OnUse(this);
                ent->isCurrentlyUsed = true;
                this->isCurrentlyUsed = true;
            }
        }

    }
}


void Character::Draw(SDL_Window* win, SDL_Renderer* ren)
{
    // Render our character.
    SDL_RenderCopyF(ren, this->texture.get(), NULL, &destinationRect);
    return;
}