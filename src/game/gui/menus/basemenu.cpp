#include "rpg/gui/menus/basemenu.h"

// Menu constructor.
BaseMenu::BaseMenu(int layer, std::string name) : GUIElement(layer, name)
{
	// Set tags.
	this->AddTag("BaseMenu");
}

// Menu deconstructor, cleans up all of our vectors.
BaseMenu::~BaseMenu()
{
	// Clear our vector. None of the pointers should have an owner anymore.
	textOptions.clear();
}

// Draw our menu to the screen.
void BaseMenu::Draw(SDL_Window* win, SDL_Renderer* ren)
{
	// Our text elements will handle drawing themselves. What we'll do here is
	// also render an outline around the destination rect of our selected element.
	auto element = textOptions[CurrentlySelectedOption];
	
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	SDL_RenderDrawRectF(ren, &element->destinationRect);
}

// Constructs a new menu option. This will add a new struct object to our array and
// also create a corrosponding text object for it. Returns a Text element pointer
// in case any further modification needs to be done.
std::shared_ptr<Text> BaseMenu::AddMenuOption(std::string name, MenuFunction function)
{
	// Construct a new struct object.
	MenuOption option = { name, function };
	options.push_back(option);

	// Create some information for our text.
	TextSettings textSettings;
	textSettings.text = name;
	textSettings.textSize = 24;
	textSettings.color = { 255, 255, 255, 255 };

	// Construct our element name.
	std::string text_elementName = this->elementName + "_option_" + std::to_string(textOptions.size() + 1);

	// Construct a new element pointer.
	std::shared_ptr<Text> ptr(new Text(this->guiLayer, text_elementName, textSettings));
	ptr->SetText(name);
	textOptions.push_back(ptr);
	return ptr;
}

// Grabs a menu option by name.
Text* BaseMenu::GetMenuOption(std::string name)
{
	for (auto& element : textOptions) { if (element->elementName == name) return element.get(); }
	return nullptr;
}

// Grabs a menu option by index.
Text* BaseMenu::GetMenuOption(int index)
{
	if (index >= textOptions.size()) return nullptr;
	return textOptions[index].get();
}

// Process keyboard input. This changes the option counter with the left and right arrow keys.
void BaseMenu::OnKeyboardInput(SDL_Keycode keyCode, bool pressed, bool released, bool repeat)
{
	// Only process a key once.
	if (repeat) return;
	if (released) return;

	// What key have we pressed?
	switch (keyCode)
	{
		case SDLK_UP: 
		{ 
			CurrentlySelectedOption--;
			if (CurrentlySelectedOption <= -1) CurrentlySelectedOption = textOptions.size() - 1;
			break; 
		}
		case SDLK_DOWN: 
		{
			CurrentlySelectedOption++;		
			if (CurrentlySelectedOption >= textOptions.size()) CurrentlySelectedOption = 0;
			break;
		}
		case SDLK_RETURN:
		{
			// Execute our function.
			options[CurrentlySelectedOption].function();
		}
	}
}