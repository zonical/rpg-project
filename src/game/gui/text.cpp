#include "rpg/rpg.h"
#include "rpg/gui/text.h"

Text::Text()
{
}

Text::Text(int layer, std::string elementName) : GUIElement(layer, elementName)
{
    this->AddTag("Text");
    this->AddTag(Tag_Renderable);
}

void Text::OnElementSpawned()
{
    // Create the texture that represents our text.
    isInitalized = true;
    CreateTextTexture();
}


bool Text::CreateTextTexture()
{
    // If we already have a texture for our text, clean it.
    if (texture != NULL) SDL_DestroyTexture(texture);

    // If we do NOT have any font loaded for this element, load a default.
    if (font == NULL)
    {
        font = EngineResources.fonts.GetFont(DEFAULT_FONT, DEFAULT_FONT_SIZE);

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
    if (this->text == "") this->text = " ";

    SDL_Surface* textSurf;

    // Create a surface from our text and text color.
    textSurf = TTF_RenderText_Blended(font.get(), text.c_str(), this->colorModifier);

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

    int w = 0;
    int h = 0;
    TTF_SizeText(font.get(), text.c_str(), &w, &h);
    destinationRect.w = w;
    destinationRect.h = h;

    // Cleanup.
    SDL_FreeSurface(textSurf);

    return true;
}

void Text::Draw(SDL_Window* win, SDL_Renderer* ren)
{
    if (texture == NULL) return;

    // Multiply the height by the amount of lines we split in the Wrapped
    // texture making function if we have a \n character.
    int count = std::count(text.begin(), text.end(), '\n');
    Renderable::Draw(win, ren);

    SDL_RenderCopyF(ren, texture, NULL, &destinationRect);
    return;
}