#include "rpg/entity.h"
#include "SDL/SDL.h"

#ifndef OVERWROLDENEMY_H_ENTITY
#define OVERWROLDENEMY_H_ENTITY

class OverworldEnemy : public Entity
{
public:
    // Initalizer.
    OverworldEnemy();

    ~OverworldEnemy()
    {
        GameEngine->RemoveEntity(this);
    }

    void OnEntitySpawned();

    // Rendering.
    void DrawEntity(SDL_Window*, SDL_Renderer*);

    // Update logic.
    void Update(float);

private:
    SDL_Rect main_image_rect;
    SDL_Texture* texture;

    float lastDistFromPlayer;
};

#endif