#pragma once
#ifndef LIGHT_H_ENTITY
#define LIGHT_H_ENTITY

#include "rpg/entity.h"
#include "SDL/SDL.h"
#include <cmath>

class Light : public Entity
{
public:
    // Initalizer.
    Light()
    {
        this->AddTag("Light");
    };
    ~Light() {};

    void OnEntityCreated()
    {
        // Loop through the doors properties and set stuff acoordingly.
        for (auto& element : tiledProperties)
        {
            // Grab the name of this element.
            auto name = element["name"].get < std::string >();

            // Set our properties.
            if (name == "color_r") this->colorModifier.r = element["value"].get< int >();
            if (name == "color_g") this->colorModifier.g = element["value"].get< int >();
            if (name == "color_b") this->colorModifier.b = element["value"].get< int >();
            if (name == "intensity") this->intensity = element["value"].get< int >();
        }
    }

    int intensity;
};

inline float CalculateDistance(float x, float y)
{
    return ceil(sqrt(pow(x, 2) + pow(y, 2)));
}

#define start1 0
#define end1 255

inline SDL_Color CalculateLightingFromDistance(float dist, SDL_Color lightColor, int lightIntensity)
{
    int r = 0, g = 0, b = 0;
    // Interpolate our distance to be in between 0.1 and 1.
    // If we don't have a cap at 0.1, we'll have completely black lighting.
    float intDist = (dist / 255) * M_PI;
    if (intDist > -0.1) intDist = -0.1;

    // These are outside our light, set them to black.
    // 80 here is just a number I pulled out of my ass, but it seems to work well.
    if (dist > -(80 * 1 - (lightIntensity / 100)))
    {
        r = lightColor.r * intDist;
        g = lightColor.g * intDist;
        b = lightColor.b * intDist;
    }

    return { (Uint8)r, (Uint8)g, (Uint8)b };
}

inline SDL_Color CalculateLightingFromEntityToTile(Entity* entity, Tile* tile, SDL_Color lightColor, int lightIntensity)
{
    // Calculate the "lighting" for this tile by checking it's position related to the player.
    float x = floor(tile->levelX - entity->levelX);
    float y = floor(tile->levelY - entity->levelY);
    float dist = -ceil(sqrt(pow(x, 2) + pow(y, 2)));

    dist = dist / ((M_PI * lightIntensity) / 100);

    return CalculateLightingFromDistance(dist, lightColor, lightIntensity);
}

inline SDL_Color CalculateLightingFromEntityToEntity(Entity* entityA, Entity* entityB, SDL_Color lightColor, int lightIntensity)
{
    // Calculate the "lighting" for this tile by checking it's position related to the player.
    float x = floor(entityA->levelX - entityB->levelX);
    float y = floor(entityA->levelY - entityB->levelY);
    float dist = -ceil(sqrt(pow(x, 2) + pow(y, 2)));

    dist = dist / ((M_PI * lightIntensity) / 100);

    return CalculateLightingFromDistance(dist, lightColor, lightIntensity);
}


inline SDL_Color CalculateLightingFromPositionToTile(float xPos, float yPos, Tile* tile, SDL_Color lightColor, int lightIntensity)
{
    // Calculate the "lighting" for this tile by checking it's position related to the player.
    float x = floor(tile->levelX - xPos);
    float y = floor(tile->levelY - yPos);
    float dist = -ceil(sqrt(pow(x, 2) + pow(y, 2)));

    dist = dist / ((M_PI * lightIntensity) / 100);

    return CalculateLightingFromDistance(dist, lightColor, lightIntensity);
}

inline SDL_Color CalculateLightingFromPositionToEntity(float xPos, float yPos, Entity* entity, SDL_Color lightColor, int lightIntensity)
{
    // Calculate the "lighting" for this tile by checking it's position related to the player.
    float x = floor(entity->levelX - xPos);
    float y = floor(entity->levelY - yPos);
    float dist = -ceil(sqrt(pow(x, 2) + pow(y, 2)));

    dist = dist / ((M_PI * lightIntensity) / 100);

    return CalculateLightingFromDistance(dist, lightColor, lightIntensity);
}

#endif