#pragma once
#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "SDL/SDL.h"

// Base class for all renderable objects in SDL.
class Renderable
{
public:
    // These x and y values are the literal X and Y values in the SDL space.
    // These will get manipulated by things like the camera.
    virtual float x() { return destinationRect.x; }
    virtual float y() { return destinationRect.y; }

    virtual float originX() { return destinationRect.x + (destinationRect.w / 2); }
    virtual float originY() { return destinationRect.y + (destinationRect.h / 2); }

    // This is local-world positioning. These values aren't changed by camera shifting.
    float levelX, levelY;

    // Size value grabbers.
    virtual float w() { return destinationRect.w; }
    virtual float h() { return destinationRect.h; }

	// This is the main rendering function for everything related to this object.
    virtual void Draw(SDL_Window* win, SDL_Renderer* ren)
    {
        // Calculate scaling.
        float new_x = (destinationRect.x + destinationRect.w / 2) - (destinationRect.w / 2 * scaleMultiplier);
        float new_y = (destinationRect.y + destinationRect.h / 2) - (destinationRect.h / 2 * scaleMultiplier);
        this->renderedRectangle = { new_x, new_y, destinationRect.w * scaleMultiplier, destinationRect.h * scaleMultiplier };

        // Set our Render color.
        SDL_SetRenderDrawColor(ren, this->colorModifier.r, this->colorModifier.g, this->colorModifier.b, this->colorModifier.a);

        // RenderCopy is handled individually by inherited classes.
    };

    // This is a float-rectangle used for literal positioning of this entity.
    SDL_FRect destinationRect;

    // This is an integer-rectangle used to determine what part of the sprite we want
    // to render.
    SDL_Rect imageRect;

    // This is the final rectangle that is rendered. This is affected by scaling. Use destinationRect for
    // positioning stuff!!!
    SDL_FRect renderedRectangle;

    // Color modifier for this Renderable. This could be multiple things such as lighting, tints, etc.
    SDL_Color colorModifier;

    // Scale modifier for this Renderable.
    float scaleMultiplier = 1.0;
};
#endif // !RENDERABLE_H
