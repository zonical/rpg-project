#include "SDL/SDL.h"
#include "rpg/renderer.h"
#include "rpg/engine.h"
#include "rpg/player.h"
#include "rpg/entities/character.h"
#include "rpg/entities/overworld_enemy.h"
#include <math.h>
#include <stdlib.h>

OverworldEnemy::OverworldEnemy() : Entity()
{
    // Set the rect sizes and positions.
    main_image_rect.w = 0;
    main_image_rect.h = 0;

    destinationRect.x = 200;
    destinationRect.y = 200;
    destinationRect.w = 64;
    destinationRect.h = 64;

    // This entity can collide with other entities.
    this->AddTag(Tag_Collision);
}

void OverworldEnemy::OnEntitySpawned()
{
    texture = GameEngine->gRenderer.GetTexture("assets/debug/test_cube.png");
}

void OverworldEnemy::Update(float dT)
{
    // Grab the player.
    Character *character = dynamic_cast<Character*>(GameEngine->gPlayer.Pawn());
    if (!character) return;

    // If the player is allowed to move, we'll do movement code here.
    if (character->HasTag("Moveable"))
    {
        float dist;

        // Calculate the vector distance between the position of the player and our current position
        // as an enemy.
        dist = sqrt(pow(this->originX() - character->originX(), 2) + pow(this->originY() - character->originY(), 2));

        if (dist < 600)
        {
            float angleR = atan2(this->originY() - character->originY(), this->originX() - character->originX());
            float angleD = floor(angleR * 180 / 3.14159265359);

            // Our angle should always be a positive number.
            if (angleD < 0.0f) angleD += 360.0f;

            float cosD = cos(angleD);
            float sinD = sin(angleD);

            float movementSpeed = 0.2;

            //if (lastDistFromPlayer < dist) movementSpeed += 0.4;

            destinationRect.x -= cosD * movementSpeed * dT;
            destinationRect.y -= sinD * movementSpeed * dT;
            
            //printf("Enemy in range! D: %f, R: %f, C: %f, S: %f\n", dist, angleD, cos(angleD), sin(angleD));
        }

        lastDistFromPlayer = dist;
    }
}

void OverworldEnemy::DrawEntity(SDL_Window* win, SDL_Renderer* ren)
{
    // Render our character.
    SDL_RenderCopyF(ren, this->texture, NULL, &destinationRect);

    // Grab the player.
    Character *character = dynamic_cast<Character*>(GameEngine->gPlayer.Pawn());
    if (!character) return;

    SDL_SetRenderDrawColor(ren, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLineF(ren, this->originX(), this->originY(), character->originX(), character->originY());
    return;
}