#include <list>
#include "rpg/gui/base.h"
#include "rpg/engine.h"

void GUI::AddElement(GUIElement* element, int layer)
{
    // Adds an entity to the global register.
    elements[layer].push_back(element);
    printf("[GUI] GUI element registered. L: %d\n", layer);
}

void GUI::RemoveElement(GUIElement* element, int layer)
{
    // Removes an entity to the global register.
    auto it = std::find(elements[layer].begin(), elements[layer].end(), element);
    if (it != elements[layer].end())
    {
        // Remove it.
        elements[layer].erase(it);
        printf("[GUI] GUI element removed. L: %d\n", layer);
    }
}

int GUI::FindFirstFreeLayer()
{
    // Returns the value of the first free list of elements.
    int bestLayerSize = -1;
    int bestLayer = 0;

    // Iterate over our layers.
    for (int i = 0; i < MAX_GUI_LAYERS; i++)
    {
        if (elements[i].size() >= bestLayerSize) continue;
        bestLayer = i;
        bestLayerSize = elements[i].size();
    }

    return bestLayer;
}

GUIElement::GUIElement(int layer)
{
    GameEngine->gGUI.AddElement(this, layer);
    this->AddTag(Tag_GUIElement);
    this->AddTag(Tag_Renderable);
}

GUIElement::~GUIElement()
{
    GameEngine->gGUI.RemoveElement(this, guiLayer);
}

void GUIElement::AddChildElement(GUIElement* element)
{
    childrenElements.push_back(element);
    element->AddTag(Tag_ChildGUIElement);
}

void GUIElement::RemoveChildElement(GUIElement* element)
{
    auto it = std::find(childrenElements.begin(), childrenElements.end(), element);
    if (it != childrenElements.end())
    {
        // Remove it.
        childrenElements.erase(it);
        element->RemoveTag(Tag_ChildGUIElement);
    }
}