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
	// Our text elements will handle drawing themselves. We'll highlight the selected one.
	auto element = textOptions[CurrentlySelectedOption];
	element->SetTextColor({ 255, 255, 0, 255 });

	// Reset our override colors for other elements.
	for (int i = 0; i < textOptions.size(); i++)
	{
		if (i == CurrentlySelectedOption) continue;
		textOptions[i]->SetTextColor({255, 255, 255, 255}); // Set to white.
	}
}

// Constructs a new menu option. This will add a new struct object to our array and
// also create a corrosponding text object for it. Returns a Text element pointer
// in case any further modification needs to be done.
std::shared_ptr<Text> BaseMenu::AddMenuOption(std::string name, MenuFunction function)
{
	// Construct a new struct object.
	MenuOption option = { name, function };
	options.push_back(option);

	// Construct our element name.
	std::string text_elementName = this->elementName + "_option_" + std::to_string(textOptions.size() + 1);

	// Construct a new element pointer.
	std::shared_ptr<Text> ptr(new Text(this->guiLayer, text_elementName));
	ptr->SetText(name);
	ptr->SetTextColor({ 255, 255, 255, 255 });
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