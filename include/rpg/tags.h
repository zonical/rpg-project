#ifndef HEADER_H_TAGS
#define HEADER_H_TAGS

#include <string>
#include <algorithm>
#include <vector>

class Taggable
{
public:
    bool HasTag(std::string tag)
    {
        auto iter = std::find(this->tags.begin(), this->tags.end(), tag);
        return !(iter == this->tags.end());
    };

    void AddTag(std::string tag)
    {
        this->tags.push_back(tag);
        return;
    };
    
    void RemoveTag(std::string tag)
    {
        std::remove(this->tags.begin(), this->tags.end(), tag);
        return;
    };

private:
    std::vector<std::string> tags;
};

#define Tag_Collision       std::string("Collision")

#define Tag_Renderable      std::string("Renderable")
#define Tag_NotRendering    std::string("NotRendering")

#define Tag_Movable         std::string("Movable")

#define Tag_Entity          std::string("Entity")
#define Tag_PlayerEntity    std::string("PlayerEntity")
#define Tag_Global          std::string("Global")

#define Tag_GUIElement      std::string("GUIElement")
#define Tag_ChildGUIElement std::string("ChildToGUIElement")
#endif