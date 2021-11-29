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
	virtual void Draw(SDL_Window*, SDL_Renderer*) {};

    // This is a float-rectangle used for literal positioning of this entity.
    SDL_FRect destinationRect;

    // This is an integer-rectangle used to determine what part of the sprite we want
    // to render.
    SDL_Rect imageRect;
};
#endif // !RENDERABLE_H
