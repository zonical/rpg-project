#pragma once
#ifndef INPUTTABLE_H
#define INPUTTABLE_H

#include "SDL/SDL.h"

// Base class for all objects that can receive keyboard or mouse input in SDL.
class Inputtable
{
public:
    virtual void OnKeyboardInput(SDL_Keycode keyCode, bool pressed, bool released, bool repeat) {};
    virtual void OnMouseMotion(int x, int y, int xrel, int yrel, Uint32 state) {};
    virtual void OnMouseButtonPressed(bool pressed, bool released, int button, int clicks, int x, int y) {};
    virtual void OnMouseWheelScrolled(int x, int y, int direction) {};
};
#endif // !INPUTTABLE_H
