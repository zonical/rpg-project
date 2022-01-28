#include "rpg/engine.h"
#include "rpg/states/overworld.h"
#include "rpg/gui/text.h"
#include "rpg/states/mainmenu.h"
// Wake up our entities and GUI elements once everything has been constructed.
void OverworldState::OnLevelLoaded()
{
    if (this->gLevel == nullptr) return;

    // Wake up our entities.
    for (auto& entityLayer : this->gLevel->lEntities)
    {
        for (auto& entity : entityLayer)
        {
            // Wake them up.
            entity->OnEntitySpawned();
        }
    }

    // Wake up our GUI elements.
    for (auto& guiLayer : gGUI.elements)
    {
        for (auto& element : guiLayer)
        {
            // Wake them up.
            element->OnElementSpawned();
        }
    }

    // Create a new debug text element and populate it with information.
    TextSettings debugTextSettings;
    debugTextSettings.wrappingWidth = DEFAULT_SCREEN_WIDTH;
    debugTextSettings.fixedHeight = 250;
    debugTextSettings.textSize = 24;

    std::shared_ptr<Text> ptr(new Text(MAX_GUI_LAYERS - 1, "debugText", debugTextSettings));
    gGUI.AddElement(ptr, ptr->guiLayer);
    ptr->OnElementSpawned();
}

// If our level is being destroyed, we'll completely remove all screen elements so we can
// have an entirely new slate for the next scene.
void OverworldState::OnLevelShutdown()
{
    if (this->gLevel == nullptr) return;

    for (auto& guiLayer : this->gGUI.elements)
    {
        guiLayer.clear();
    }
}

// Begin transitioning to our new level. Our gLevelTransData struct holds the information
// we need to begin creating the new level, so we'll begin that straight away.
void OverworldState::PerformLevelTransition()
{
    // Delete our current level.
    delete gLevel;

    // Create our new level and load in everything we need.
    gLevel = new Level();
    gLevel->LoadLevel(gLevelTransData.new_level);

    // Now that we've loaded in our level, we have access to all of our entities.
    // Find our level landmark and set our player position to be that point.
    for (auto& entLayer : gLevel->lEntities)
    {
        for (auto& entity : entLayer)
        {
            // Is this entity a landmark and has the required name?
            if (entity->classname == "landmark" &&
                entity->targetname == gLevelTransData.landmark_name)
            {
                // Set our Character position to the position of this landmark.
                Character* character = gLevel->GetCharacter();
                if (character != nullptr)
                {
                    // Do a little offset here so our position is at the origin
                    // of the character sprite.
                    character->ForcePosition(
                        entity->levelX - (character->w() / 2),
                        entity->levelY - (character->h() / 2));
                }
                break;
            }
        }
    }

    // Now that we've completed our level transition, reset our struct so we can
    // resume normal gameplay.
    gLevelTransData.transitionFlag = false;
    gLevelTransData.new_level = "";
    gLevelTransData.landmark_name = "";

    // Fade from black.
    EngineResources.FadeFromBlack(LEVEL_TRANSITION_FADE);
}

// Update everything in this state including entities and the GUI.
void OverworldState::Update(float dT)
{
    // If we're being marked for level transition, delete our current level and start transitioning
    // to the next one. Don't update anything else here.
    if (gLevel != nullptr && gLevelTransData.transitionFlag)
    {
        PerformLevelTransition();
        return;
    }

    // Update everything inside our level.
    if (gLevel != nullptr)
    {
        gLevel->LevelUpdate(dT);
    }

    // Run logic for all of our GUI elements.
    for (auto& layer : gGUI.elements)
    {
        for (auto& element : layer)
        {
            if (element.get() == nullptr) continue;
            element->Update(dT);
        }
    }

    // If we have debug mode enabled, we'll update our debug text element.
    if (GameEngine->debugModeEnabled)
    {
        auto element = dynamic_cast<Text*>(gGUI.GetElement("debugText"));
        if (element != nullptr)
        {
            // Update our text with information.
            std::stringstream fpsText;

            int guiElementCount = 0;
            for (auto& layer : gGUI.elements) guiElementCount += layer.size();

            int entityCount = 0;
            for (auto& layer : gLevel->lEntities) entityCount += layer.size();

            fpsText << "FPS: " << floor(GameEngine->fps) << '\n'
                << "Player xPos: " << gLevel->GetCharacter()->levelX << '\n'
                << "Player yPos: " << gLevel->GetCharacter()->levelY << '\n'
                << "Frame: " << GameEngine->frame << '\n'
                << "Elapsed Time: " << GameEngine->elapsedTime << '\n'
                << "Entity Count: " << entityCount << '\n'
                << "GUI Element Count: " << guiElementCount << '\n'
                ;

            element->SetText(fpsText.str());
        }
    }
    else
    {
        // Set our text to nothing.
        auto element = dynamic_cast<Text*>(gGUI.GetElement("debugText"));
        if (element != nullptr)
        {
            element->SetText("");
        }
    }
}

bool CollisionCheckF(SDL_FRect a, SDL_FRect b)
{
    // %s1 = us
    // %s2 = other
    float left1, left2;
    float right1, right2;
    float top1, top2;
    float bottom1, bottom2;

    left1 = a.x;
    right1 = a.x + a.w;
    top1 = a.y;
    bottom1 = a.y + a.h;

    left2 = b.x;
    right2 = b.x + b.w;
    top2 = b.y;
    bottom2 = b.y + b.h;

    if (bottom1 <= top2) return false;
    if (top1 >= bottom2) return false;
    if (right1 <= left2) return false;
    if (left1 >= right2) return false;

    // We're not colliding with anything.
    return true;
}

void OverworldState::Draw(SDL_Window* win, SDL_Renderer* ren)
{
    if (!gLevel) return;

    // Offset our collision rectangles by the camera.
    for (auto& obj : gLevel->lCollisionR)
    {
        obj.collisionRect.x = obj.levelX - EngineResources.camera.x;
        obj.collisionRect.y = obj.levelY - EngineResources.camera.y;
    }

    // There can be up to 16 different layers for both entities and tiles. To allow for
    // tiles to overlap entities (and vice versa), we'll render the tile layer FIRST and then
    // render entities from the bottom 0 layer to the top layer 15.
    for (int i = 0; i < MAX_TILE_LAYERS; i++)
    {
        // Render our tiles.
        for (auto& tile : gLevel->lTiles[i])
        {
            // Manipulate the destinationRect of this tile to be offset by the camera.
            tile->destinationRect.x = tile->levelX - EngineResources.camera.x;
            tile->destinationRect.y = tile->levelY - EngineResources.camera.y;

            SDL_FRect checkRect = { tile->levelX, tile->levelY, tile->destinationRect.w, tile->destinationRect.h };

            // Are we in the camera's view?
            if (!CollisionCheckF(EngineResources.camera, checkRect)) continue;

            // Call the tiles' render function.
            if (tile->HasTag(Tag_Renderable) || !tile->HasTag(Tag_NotRendering))
            {
                tile->Draw(win, ren);
            }
        }

        // Render our entities.
        for (auto& entity : gLevel->lEntities[i])
        {
            // Manipulate the destinationRect of this tile to be offset by the camera.
            entity->destinationRect.x = entity->levelX - EngineResources.camera.x;
            entity->destinationRect.y = entity->levelY - EngineResources.camera.y;

            SDL_FRect checkRect = { entity->levelX, entity->levelY, entity->destinationRect.w, entity->destinationRect.h };

            // Are we in the camera's view?
            if (!CollisionCheckF(EngineResources.camera, checkRect)) continue;

            // Call the entities render function.
            if (entity->HasTag(Tag_Renderable) || !entity->HasTag(Tag_NotRendering))
            {
                entity->Draw(win, ren);
            }
        }
    }

    // Render the GUI.
    for (auto& layer : gGUI.elements)
    {
        // Call the entities render function.
        for (auto& element : layer)
        {
            if (element->HasTag(Tag_Renderable) || !element->HasTag(Tag_NotRendering))
            {
                element->Draw(win, ren);
            }

        }
    }
    return;
}

void OverworldState::OnKeyboardInput(SDL_Keycode keyCode, bool pressed, bool released, bool repeat)
{
    if (keyCode == SDLK_ESCAPE)
    {
        GameEngine->ChangeGameState(State_MainMenu);
    }

    if (gLevel != nullptr)
    {
        // Tell all of our entities that a key has been pressed.
        for (auto& entityLayer : gLevel->lEntities)
        {
            for (auto& entity : entityLayer)
            {
                entity->OnKeyboardInput(keyCode, pressed, !pressed, repeat);
            }
        }
    }

    // Tell all of our elements that a key has been released.
    for (auto& layer : gGUI.elements)
    {
        for (auto& element : layer)
        {
            // Call the elements keyboard input function.
            element->OnKeyboardInput(keyCode, pressed, !pressed, repeat);
        }
    }
}
