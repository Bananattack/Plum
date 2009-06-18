#include "../plum.h"

namespace Plum
{
/*	GameControl::GameControl()
	{
		init("", -1, -1);
	}

	GameControl::GameControl(std::string name)
	{
		init(name, -1, -1);
	}

	GameControl::GameControl(std::string name, int keyboardButton)
	{
		init(name, keyboardButton, -1);
	}

	GameControl::GameControl(std::string name, int keyboardButton, int joystickButton)
	{
		init(name, keyboardButton, joystickButton);
	}

	void GameControl::init(std::string name, int keyboardButton, int joystickButton)
	{
		this->name = name;
		this->keyboardButton = keyboardButton;
		this->joystickButton = joystickButton;
		pressed = false;
	}

	bool GameControl::isPressed()
	{
		return pressed;
	}

	void GameControl::setPressed(bool pressed)
	{
		this->pressed = pressed;
	}

	void GameControl::handleEvent(SDL_Event event)
	{
		if(event.type == SDL_KEYDOWN && event.key.keysym.sym == keyboardButton)
		{
			printf("PRESSED %d\n", event.key.keysym.sym);
			pressed = true;
		}
		if(event.type == SDL_KEYUP && event.key.keysym.sym == keyboardButton)
		{
			printf("UNPRESSED %d\n", event.key.keysym.sym);
			pressed = false;
		}
	}*/
}