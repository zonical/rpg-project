#ifndef GUI_BASE_H
#define GUI_BASE_H

#include "SDL/SDL.h"
#include "rpg/tags.h"

#include <list>
#include <array>

#define MAX_GUI_LAYERS 16

class GUIElement : public Taggable
{
public:
    // Default constructors and deconstructors.
    // Elements that inherit this class will be able to call these
    // and save having to copy and paste the same create and destroy
    // code over and over for each class.
    GUIElement() {};
    GUIElement(int layer);
    ~GUIElement();

    // The current layer this element is on.
    int guiLayer;

    // If there are any elements that want to be considered a child to this one,
    // then they should be added to this list. They should still be added to the
    // global GameEngine->gGUI object, but if an element wants a local reference
    // (e.g pressing a button destroys an element) then it can.
    std::list<GUIElement*> childrenElements;

    void AddChildElement(GUIElement*);
    void RemoveChildElement(GUIElement*);

    // This is the main rendering function for everything related to this element.
    virtual void DrawElement(SDL_Window*, SDL_Renderer*) {};

    // Updating and logic.
    virtual void Update(float deltaTime) {};
    float nextUpdate;   // If there is to be a delay in processing the logic in the
                        // Think function, have an already defined variable for it.

    virtual void OnKeyboardInput(SDL_Keycode keyCode, bool pressed, bool released, bool repeat) {};
    virtual void OnMouseMotion(int x, int y, int xrel, int yrel, Uint32 state) {};
    virtual void OnMouseButtonPressed(bool pressed, bool released, int button, int clicks, int x, int y) {};
    virtual void OnMouseWheelScrolled(int x, int y, int direction) {};

    // Positioning.

    // X value of this element.
    float x() { return destinationRect.x; }
    float y() { return destinationRect.y; }

    float originX() { return destinationRect.x + (destinationRect.w / 2); }
    float originY() { return destinationRect.y + (destinationRect.h / 2); }

    // This is a float-rectangle used for positioning of this element.
    SDL_FRect    destinationRect;

    // Size value grabbers.
    float w() { return destinationRect.w; }
    float h() { return destinationRect.h; }
};

class GUI
{
public:
    // An array containing a list of all of our elements, separated by layer.
    // Being on layer 0 means you're at the bottom of the screen, layer MAX_GUI_LAYERS - 1
    // means you're at the top of the screen.
    std::array < std::list<GUIElement*>, MAX_GUI_LAYERS > elements;

    // Returns the value of the first free list of elements.
    int FindFirstFreeLayer();

    void AddElement(GUIElement* element, int layer);
    void RemoveElement(GUIElement* element, int layer);
};

#endif