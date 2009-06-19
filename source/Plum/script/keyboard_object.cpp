#include "../plum.h"


// Condenses the following:
// plum.key[key_index] = plum._Input(engine key AT key_index);
#define BIND_KEY(key_index, name) \
		lua_pushcfunction(L, inputConstructor); \
		lua_pushlightuserdata(L, &(engine->key[key_index])); \
		lua_call(L, 1, 1); \
		lua_setfield(L, -2, name);

namespace Plum
{
	namespace Script
	{
		void initKeyboardClass(lua_State* L)
		{
			lua_CFunction inputConstructor = NULL;

			// Push plum namespace.
			lua_getglobal(L, "plum");

			// Store the reference to the plum._Input constructor
			lua_getfield(L, -1, "_Input");
			inputConstructor = lua_tocfunction(L, -1);
			lua_pop(L, 1);

			// t = {}
			lua_newtable(L);

			// plum.key = t
			lua_pushstring(L, "key");
			lua_pushvalue(L, -2);
			lua_settable(L, -4);

			// Oh boy, here comes the the fun part.
			BIND_KEY(KEY_BACKSPACE, "Backspace");
			BIND_KEY(KEY_TAB, "Tab");
			BIND_KEY(KEY_ENTER, "Enter");
			BIND_KEY(KEY_PAUSE, "Pause");
			BIND_KEY(KEY_ESCAPE, "Escape");
			BIND_KEY(KEY_SPACE, "Space");
			BIND_KEY(KEY_EXCLAIM, "Exclaim");
			BIND_KEY(KEY_QUOTE, "Quote");
			BIND_KEY(KEY_HASH, "Hash");
			BIND_KEY(KEY_DOLLAR, "Dollar");
			BIND_KEY(KEY_AMPERSAND, "Ampersand");
			BIND_KEY(KEY_APOSTROPHE, "Apostrophe");
			BIND_KEY(KEY_LEFTPAREN, "LeftParen");
			BIND_KEY(KEY_RIGHTPAREN, "RightParen");
			BIND_KEY(KEY_ASTERISK, "Asterisk");
			BIND_KEY(KEY_PLUS, "Plus");
			BIND_KEY(KEY_COMMA, "Comma");
			BIND_KEY(KEY_MINUS, "Minus");
			BIND_KEY(KEY_PERIOD, "Period");
			BIND_KEY(KEY_SLASH, "Slash");
			BIND_KEY(KEY_0, "0");
			BIND_KEY(KEY_1, "1");
			BIND_KEY(KEY_2, "2");
			BIND_KEY(KEY_3, "3");
			BIND_KEY(KEY_4, "4");
			BIND_KEY(KEY_5, "5");
			BIND_KEY(KEY_6, "6");
			BIND_KEY(KEY_7, "7");
			BIND_KEY(KEY_8, "8");
			BIND_KEY(KEY_9, "9");
			BIND_KEY(KEY_COLON, "Colon");
			BIND_KEY(KEY_SEMICOLON, "Semicolon");
			BIND_KEY(KEY_LESSTHAN, "LessThan");
			BIND_KEY(KEY_EQUALS, "Equals");
			BIND_KEY(KEY_GREATERTHAN, "GreaterThan");
			BIND_KEY(KEY_QUESTION, "Question");
			BIND_KEY(KEY_AT, "At");
			BIND_KEY(KEY_LEFTBRACKET, "LeftBracket");
			BIND_KEY(KEY_BACKSLASH, "Backslash");
			BIND_KEY(KEY_RIGHTBRACKET, "RightBracket");
			BIND_KEY(KEY_CARET, "Caret");
			BIND_KEY(KEY_UNDERSCORE, "Underscore");
			BIND_KEY(KEY_TILDE, "Tilde");
			BIND_KEY(KEY_BACKQUOTE, "Backquote");
			BIND_KEY(KEY_GRAVE, "Grave");
			BIND_KEY(KEY_A, "A");
			BIND_KEY(KEY_B, "B");
			BIND_KEY(KEY_C, "C");
			BIND_KEY(KEY_D, "D");
			BIND_KEY(KEY_E, "E");
			BIND_KEY(KEY_F, "F");
			BIND_KEY(KEY_G, "G");
			BIND_KEY(KEY_H, "H");
			BIND_KEY(KEY_I, "I");
			BIND_KEY(KEY_J, "J");
			BIND_KEY(KEY_K, "K");
			BIND_KEY(KEY_L, "L");
			BIND_KEY(KEY_M, "M");
			BIND_KEY(KEY_N, "N");
			BIND_KEY(KEY_O, "O");
			BIND_KEY(KEY_P, "P");
			BIND_KEY(KEY_Q, "Q");
			BIND_KEY(KEY_R, "R");
			BIND_KEY(KEY_S, "S");
			BIND_KEY(KEY_T, "T");
			BIND_KEY(KEY_U, "U");
			BIND_KEY(KEY_V, "V");
			BIND_KEY(KEY_W, "W");
			BIND_KEY(KEY_X, "X");
			BIND_KEY(KEY_Y, "Y");
			BIND_KEY(KEY_Z, "Z");
			BIND_KEY(KEY_DELETE, "Delete");
			BIND_KEY(KEY_NUMPAD_0, "Numpad0");
			BIND_KEY(KEY_NUMPAD_1, "Numpad1");
			BIND_KEY(KEY_NUMPAD_2, "Numpad2");
			BIND_KEY(KEY_NUMPAD_3, "Numpad3");
			BIND_KEY(KEY_NUMPAD_4, "Numpad4");
			BIND_KEY(KEY_NUMPAD_5, "Numpad5");
			BIND_KEY(KEY_NUMPAD_6, "Numpad6");
			BIND_KEY(KEY_NUMPAD_7, "Numpad7");
			BIND_KEY(KEY_NUMPAD_8, "Numpad8");
			BIND_KEY(KEY_NUMPAD_9, "Numpad9");
			BIND_KEY(KEY_NUMPAD_PERIOD, "NumpadPeriod");
			BIND_KEY(KEY_NUMPAD_DIVIDE, "NumpadDivide");
			BIND_KEY(KEY_NUMPAD_MULTIPLY, "NumpadMultiply");
			BIND_KEY(KEY_NUMPAD_MINUS, "NumpadMinus");
			BIND_KEY(KEY_NUMPAD_PLUS, "NumpadPlus");
			BIND_KEY(KEY_NUMPAD_ENTER, "NumpadEnter");
			BIND_KEY(KEY_NUMPAD_EQUALS, "NumpadEquals");
			BIND_KEY(KEY_UP, "Up");
			BIND_KEY(KEY_DOWN, "Down");
			BIND_KEY(KEY_LEFT, "Left");
			BIND_KEY(KEY_RIGHT, "Right");
			BIND_KEY(KEY_INSERT, "Insert");
			BIND_KEY(KEY_HOME, "Home");
			BIND_KEY(KEY_END, "End");
			BIND_KEY(KEY_PAGEUP, "PageUp");
			BIND_KEY(KEY_PAGEDOWN, "PageDown");
			BIND_KEY(KEY_F1, "F1");
			BIND_KEY(KEY_F2, "F2");
			BIND_KEY(KEY_F3, "F3");
			BIND_KEY(KEY_F4, "F4");
			BIND_KEY(KEY_F5, "F5");
			BIND_KEY(KEY_F6, "F6");
			BIND_KEY(KEY_F7, "F7");
			BIND_KEY(KEY_F8, "F8");
			BIND_KEY(KEY_F9, "F9");
			BIND_KEY(KEY_F10, "F10");
			BIND_KEY(KEY_F11, "F11");
			BIND_KEY(KEY_F12, "F12");
			BIND_KEY(KEY_NUMLOCK, "NumLock");
			BIND_KEY(KEY_CAPSLOCK, "CapsLock");
			BIND_KEY(KEY_SCROLLOCK, "ScrollLock");
			BIND_KEY(KEY_SCROLLOCK, "ScrollLock");
			BIND_KEY(KEY_RSHIFT, "RightShift");
			BIND_KEY(KEY_LSHIFT, "LeftShift");
			BIND_KEY(KEY_RSHIFT, "RightCtrl");
			BIND_KEY(KEY_LSHIFT, "LeftCtrl");
			BIND_KEY(KEY_RSHIFT, "RightAlt");
			BIND_KEY(KEY_LSHIFT, "LeftAlt");
			BIND_KEY(KEY_BREAK, "Break");

			// Pop plum.key table. (Whew.)
			lua_pop(L, 1);
		
			// Pop plum namespace.
			lua_pop(L, 1);
		}
	}
}