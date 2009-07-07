#pragma once

namespace Plum
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