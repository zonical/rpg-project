#include <list>
#include "rpg/gui/base.h"
#include "rpg/engine.h"

void GUI::AddElement(std::shared_ptr<GUIElement> element, int layer)
{
    elements[layer].push_back(std::move(element));
    std::cout << "[GUI] GUI element registered." << std::endl;
}

void GUI::RemoveElement(std::shared_ptr<GUIElement> element, int layer)
{
    auto it = std::find(elements[layer].begin(), elements[layer].end(), element);
    if (it != elements[layer].end())
    {
        // Remove it.
        elements[layer].erase(it);
    }
}

void GUI::RemoveElement(std::string elementName, int layer)
{
    std::list<std::shared_ptr<GUIElement>>::iterator it;
    for (it = elements[layer].begin(); it != elements[layer].end(); ++it) 
    {
        // Does this name match?
        if (it->get()->elementName == elementName)
        {
            // Yeet it.
            elements[layer].erase(it);
            return;
        }
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

GUIElement::GUIElement(int layer, std::string elementName)
{
    this->AddTag(Tag_GUIElement);
    this->AddTag(Tag_Renderable);
    this->elementName = elementName;
    this->guiLayer = layer;
}

GUIElement::~GUIElement()
{
}