#ifndef PLUM_INPUT_H
#define PLUM_INPUT_H

namespace plum
{
    class Input
    {
        protected:
            Input() {}
        public:
            virtual ~Input() {}
            virtual bool isPressed() = 0;
            virtual void setPressed(bool pressed) = 0;
    };
}

#include "keyboard.h"
#include "mouse.h"

#endif