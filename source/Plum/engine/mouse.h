#ifndef PLUM_MOUSE_H
#define PLUM_MOUSE_H

namespace Plum
{
	const int MOUSE_LEFT = 0;
	const int MOUSE_MIDDLE = 1;
	const int MOUSE_RIGHT = 2;

	class MouseInput : public Input
	{
		private:
			bool pressed;
		public:
			MouseInput() { pressed = false; }
			virtual bool isPressed() { return pressed; }
			virtual void setPressed(bool pressed) { this->pressed = pressed; }
	};

	struct Mouse
	{
		double x, y;
		int wheelPosition;
		MouseInput button[3];

		Mouse()
			: x(0), y(0), wheelPosition(0)
		{
		}
	};
}

#endif