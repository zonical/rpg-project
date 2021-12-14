#ifndef GUI_BASE_H
#define GUI_BASE_H

#include "SDL/SDL.h"
#include "rpg/base/taggable.h"
#include "rpg/base/renderable.h"
#include "rpg/base/inputtable.h"

#include <list>
#include <array>
#include <memory>

#define MAX_GUI_LAYERS 16

class GUIElement 
    : public Taggable, public Renderable, public Inputtable
{
public:
    std::string elementName;

    // Default constructors and deconstructors.
    // Elements that inherit this class will be able to call these
    // and save having to copy and paste the same create and destroy
    // code over and over for each class.
    GUIElement() {};
    GUIElement(int layer, std::string name);
    ~GUIElement();

    virtual void OnElementSpawned() {};

    // The current layer this element is on.
    int guiLayer;

    // Updating and logic.
    virtual void Update(float deltaTime) {};
    float nextUpdate;   // If there is to be a delay in processing the logic in the
                        // Think function, have an already defined variable for it.
};

class GUI
{
public:
    // An array containing a list of all of our elements, separated by layer.
    // Being on layer 0 means you're at the bottom of the screen, layer MAX_GUI_LAYERS - 1
    // means you're at the top of the screen.
    std::array < std::list < std::shared_ptr<GUIElement> >, MAX_GUI_LAYERS > elements;

    // Returns the value of the first free list of elements.
    int FindFirstFreeLayer();

    void AddElement(std::shared_ptr<GUIElement> element, int layer);
    void RemoveElement(std::shared_ptr<GUIElement> element, int layer);
    void RemoveElement(std::string elementName, int layer);
};

#endif