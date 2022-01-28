#include <string>
#include <array>
#include <algorithm>
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "rpg/gui/base.h"
#include "rpg/gui/text.h"

Text::Text()
{
}

Text::Text(int layer, std::string elementName, TextSettings settings) : GUIElement(layer, elementName)
{
    this->AddTag("Text");
    this->AddTag(Tag_Renderable);
    this->text = settings.text;
    this->textColor = settings.color;
    this->destinationRect.x = settings.x;
    this->destinationRect.y = settings.y;
    this->wrappingWidth = settings.wrappingWidth;
    this->destinationRect.h = settings.fixedHeight;
    this->destinationRect.w = settings.fixedWidth;
    this->textSize = settings.textSize;

    if (this->destinationRect.w > 0) customWidth = true;
    if (this->destinationRect.h > 0) customHeight = true;
}

void Text::OnElementSpawned()
{
    // Create the texture that represents our text.
    CreateTextTexture();
}


bool Text::CreateTextTexture()
{
    // If we already have a texture for our text, clean it.
    if (texture != NULL)
    {
        SDL_DestroyTexture(texture);
    }

    // If we do NOT have any font loaded for this element, load a default.
    if (font == NULL)
    {
        font = EngineResources.fonts.GetFont(DEFAULT_FONT, this->textSize);

        // If our font STILL equals null, cancel making the texture.
        if (font == NULL)
        {
            printf("[TEXT-ERROR] Failed to grab font.\n");
            return false;
        }

    }

    // Because of SDL_tff crud, if we don't set a default string and leave it
    // as "", it will throw a "zero width" error. We'll just set it to a space
    // here and it can get overriden later.
    if (this->text == "")
    {
        this->text = " ";
    }

    SDL_Surface* textSurf;

    // Create a surface from our text and text color.
    // If we have an invalid surface, return an error here.
    if (text.find("\n") != std::string::npos)
    {
        textSurf = TTF_RenderText_Blended_Wrapped(font.get(), text.c_str(), textColor, wrappingWidth);
    }
    else
    {
        textSurf = TTF_RenderText_Blended(font.get(), text.c_str(), textColor);
    }

    if (textSurf == NULL)
    {   
        printf("[TEXT-ERROR] Failed to create surface for text message: \"%s\" (%s)\n", text.c_str(), SDL_GetError());
        return false;
    }

    // Create a texture from the surface.
    // If we don't have a renderer, return.
    if (EngineResources.renderer == NULL)
    {
        printf("[TEXT-ERROR] Failed to create surface for text message, missing Renderer: %s\n", text.c_str());
        return false;
    }

    texture = SDL_CreateTextureFromSurface(EngineResources.renderer, textSurf);
    if (texture == NULL)
    {
        // If we have an invalid surface, return an error here.
        printf("[TEXT-ERROR] Failed to create texture for text message: \"%s\" (%s)\n", text.c_str(), SDL_GetError());
        SDL_FreeSurface(textSurf);
        return false;
    }


    // Set the size of our rectangle.
    if (!customWidth)   destinationRect.w = textSurf->w;
    if (!customHeight)  destinationRect.h = textSurf->h;

    // Cleanup.
    SDL_FreeSurface(textSurf);

    return true;
}

void Text::Draw(SDL_Window* win, SDL_Renderer* ren)
{
    // If we are having to create a new surface every frame (this might need to be done
    // if we're doing something like having dynamic text overtime like in a dialouge box)
    // we'll do it here.
    if (!isTextStatic) CreateTextTexture();
    if (texture == NULL) return;

    // Multiply the height by the amount of lines we split in the Wrapped
    // texture making function if we have a \n character.
    int count = std::count(text.begin(), text.end(), '\n');
    if (count > 0 && !customHeight) destinationRect.h *= count;

    SDL_RenderCopyF(ren, texture, NULL, &destinationRect);
    return;
}