#include "input.h"

namespace plum
{
    Input::Input()
        : pressed(false)
    {
    }

    Input::~Input()
    {
    }

    bool Input::isPressed()
    {
        return pressed;
    }

    void Input::setPressed(bool value)
    {
        pressed = value;
    }
}