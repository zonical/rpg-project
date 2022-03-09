#include "rpg/engine.h"
#include "rpg/states/overworld.h"
#include "rpg/gui/text.h"
#include "rpg/states/mainmenu.h"
#include "rpg/entities/light.h"

#include <algorithm>
#include <cmath>

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

    std::shared_ptr<Text> fpsText(new Text(MAX_GUI_LAYERS - 1, "fpsText"));
    fpsText->SetTextColor({ 255, 255, 255, 255 });
    gGUI.AddElement(fpsText, fpsText->guiLayer);
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
        auto element = dynamic_cast<Text*>(gGUI.GetElement("fpsText"));
        if (element != nullptr)
        {
            // Update our text with information.
            std::stringstream fpsText;
            element->SetText(std::to_string(floor(GameEngine->fps)));
        }
    }
    else
    {
        // Set our text to nothing.
        auto element = dynamic_cast<Text*>(gGUI.GetElement("fpsText"));
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
            if (this->tileCullingType == CULLING_STANDARD || this->tileCullingType == CULLING_NO_LIGHTS)
            {
                if (!CollisionCheckF(EngineResources.camera, checkRect)) continue;
            }
            
            // Call the tiles' render function.
            if (tile->HasTag(Tag_Renderable) || !tile->HasTag(Tag_NotRendering))
            {
                // Get the position of our mouse.
                int x, y;
                SDL_GetMouseState(&x, &y);
                
                // Calculate our lighting.
                SDL_Color finalLight = CalculateLightingFromPositionToTile(x + EngineResources.camera.x, y + EngineResources.camera.y, tile.get(), { 255, 255, 255 }, 60);

                // Add our existing tile color data.
                finalLight.r = (int)std::min(tile->colorModifier.r + finalLight.r, 255);
                finalLight.g = (int)std::min(tile->colorModifier.g + finalLight.g, 255);
                finalLight.b = (int)std::min(tile->colorModifier.b + finalLight.b, 255);

                // If our tile is completely black, don't bother rendering it.
                if (this->tileCullingType == CULLING_STANDARD)
                {
                    if (finalLight.r == 0 && finalLight.g == 0 && finalLight.b == 0) continue;
                }

                // Are we rendering some debug properties?
                switch (this->lightingRenderType)
                {
                    case LIGHTING_FILL: // Fills all tiles with just filled rectangles showing the light.
                    {
                        SDL_SetRenderDrawColor(ren, finalLight.r, finalLight.g, finalLight.b, 255);
                        SDL_RenderFillRectF(ren, &tile->destinationRect);
                        break;
                    }
                    case LIGHTING_BORDER:
                    {
                        SDL_SetRenderDrawColor(ren, finalLight.r, finalLight.g, finalLight.b, 255);
                        SDL_RenderDrawRectF(ren, &tile->destinationRect);
                        break;
                    }
                }

                SDL_SetTextureColorMod(tile->texture.get(), finalLight.r, finalLight.g, finalLight.b);

                // Draw our final tile.
                tile->scaleMultiplier = this->scaleMultiplier;
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
                // Draw our final entity.
                entity->scaleMultiplier = this->scaleMultiplier;
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
    if (pressed && !repeat)
    {
        switch (keyCode)
        {
            case SDLK_ESCAPE: GameEngine->ChangeGameState(State_MainMenu); break;
            case SDLK_TAB: selection++; break;
            case SDLK_LSHIFT: selection--; break;

            case SDLK_1:
            {
                if (selection == SELECT_LIGHTING)
                {
                    this->lightingRenderType = LIGHTING_STANDARD; break;
                }
                else if (selection == SELECT_TILECULLING)
                {
                    this->tileCullingType = CULLING_STANDARD; break;
                }
            }
            case SDLK_2:
            {
                if (selection == SELECT_LIGHTING)
                {
                    this->lightingRenderType = LIGHTING_DISABLED; break;
                }
                else if (selection == SELECT_TILECULLING)
                {
                    this->tileCullingType = CULLING_NO_LIGHTS; break;
                }
            }
            case SDLK_3:
            {
                if (selection == SELECT_LIGHTING)
                {
                    this->lightingRenderType = LIGHTING_BORDER; break;
                }
                else if (selection == SELECT_TILECULLING)
                {
                    this->tileCullingType = CULLING_DISABLED; break;
                }
            }

            case SDLK_4:
            {
                if (selection == SELECT_LIGHTING)   this->lightingRenderType = LIGHTING_FILL; break;
        }
        }
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

void OverworldState::OnMouseWheelScrolled(int x, int y, int direction)
{

}