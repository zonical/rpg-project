#include "SDL/SDL.h"
#include "rpg/resources.h"
#include "rpg/engine.h"
#include "rpg/entities/npc.h"
#include "rpg/gamestate.h"
#include "rpg/states/overworld.h"

NPCEntity::NPCEntity() : Entity()
{
    // This entity can collide with other entities.
    this->AddTag(Tag_Renderable);
    this->AddTag(Tag_Collision);
}

NPCEntity::~NPCEntity()
{
}

void NPCEntity::OnEntitySpawned()
{
    // Loop through the NPC's properties and set stuff acoordingly.
    for (auto& element : tiledProperties)
    {
        // Grab the name of this element.
        auto name = element["name"].get < std::string >();

        // Set our properties.
        if (name == "npc_name") npcName = element["value"].get< std::string >();
        if (name == "npc_sprite") npctexture = EngineResources.textures.GetTexture(element["value"].get< std::string >());
        if (name == "npc_use_dist") useDistance = element["value"].get< float >();
        if (name == "npc_action_file") npcActions = GameEngine->LoadJSON(element["value"].get< std::string >());
    }
}

void NPCEntity::Update(float dT)
{
    if (!isCurrentlyUsed) return;
    
    // Are we currently using a textbox?
    if (this->textbox.get() != nullptr)
    {     
        // Is it completely finished and the user has exited out of it?
        if (this->textbox->IsDialogueFinished())
        {
            State_Overworld->gGUI.RemoveElement(textbox->elementName, textbox->guiLayer);
            this->textbox.reset();
            OnUseFinished(useActivator);
        }
    }
}

void NPCEntity::OnUse(Entity* activator)
{
    try
    {
        // Check to see if we have an "onUse" property in our NPC actions
        // file. If we do, perform some cool actions.
        if (npcActions.contains("onUse"))
        {
            useActivator = activator;

            // If we're a character, stop moving us.
            if (activator->HasTag("Character"))
            {
                activator->AddTag("DontMove");
            }

            // Grab this object.
            auto useObj = npcActions["onUse"].get<json>();
            auto action = useObj["action"].get<std::string>();

            // Open a dialogue box with a message.
            if (action == "dialogueOpen")
            {  
                // Don't open another if we already have one!
                if (this->textbox != nullptr) return;

                // Get textbox type and construct a new textbox.
                auto textboxType = useObj["dialogueBoxType"].get<int>();

                std::string elementName = npcName + "_textbox";
                std::shared_ptr<Textbox> ptr(new Textbox(State_Overworld->gGUI.FindFirstFreeLayer(), elementName, textboxType));
                textbox = ptr;
                State_Overworld->gGUI.AddElement(ptr, ptr->guiLayer);
                textbox->OnElementSpawned();

                // Load our dialogue.
                auto dialogue = useObj["messageFile"].get<std::string>();
                this->textbox->LoadDialogue(dialogue);
            }
        }
    }
    catch (std::exception& exception)
    {
        printf("[NPC] Error! %s\n", exception.what());
    }

}

void NPCEntity::OnUseFinished(Entity* activator)
{
    // If we're a character, allow movement.
    if (activator->HasTag("Character"))
    {
        activator->RemoveTag("DontMove");
        activator->isCurrentlyUsed = false;
        this->isCurrentlyUsed = false;
    }
    useActivator = nullptr;
}


void NPCEntity::Draw(SDL_Window* win, SDL_Renderer* ren)
{
    // Render our character.
    Renderable::Draw(win, ren);
    SDL_RenderCopyF(ren, this->npctexture.get(), NULL, &renderedRectangle);
    return;
}