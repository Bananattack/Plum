#include "../plum.h"
#include "../engine/engine.h"
#include "script.h"

namespace plum
{
    namespace
    {
        void bind(lua_State* L, lua_CFunction constructor, int index, const char* name)
        {
            // plum.key[index] = plum.Input(keyref);
            lua_pushcfunction(L, constructor);
            lua_pushlightuserdata(L, &(script::instance(L).engine().key[index]));
            lua_call(L, 1, 1);
            lua_setfield(L, -2, name);
        }
    }

    namespace script
    {
        void initKeyboardModule(lua_State* L)
        {
            // Push plum namespace.
            lua_getglobal(L, "plum");

            // Store the reference to the plum._Input constructor
            lua_getfield(L, -1, "_Input");
            auto constructor = lua_tocfunction(L, -1);
            lua_pop(L, 1);

            // t = {}
            lua_newtable(L);

            // plum.key = t
            lua_pushstring(L, "key");
            lua_pushvalue(L, -2);
            lua_settable(L, -4);

            // Oh boy, here comes the the fun part.
            bind(L, constructor, KEY_BACKSPACE, "Backspace");
            bind(L, constructor, KEY_TAB, "Tab");
            bind(L, constructor, KEY_ENTER, "Enter");
            bind(L, constructor, KEY_PAUSE, "Pause");
            bind(L, constructor, KEY_ESCAPE, "Escape");
            bind(L, constructor, KEY_SPACE, "Space");
            bind(L, constructor, KEY_EXCLAIM, "Exclaim");
            bind(L, constructor, KEY_QUOTE, "Quote");
            bind(L, constructor, KEY_HASH, "Hash");
            bind(L, constructor, KEY_DOLLAR, "Dollar");
            bind(L, constructor, KEY_AMPERSAND, "Ampersand");
            bind(L, constructor, KEY_APOSTROPHE, "Apostrophe");
            bind(L, constructor, KEY_LEFTPAREN, "LeftParen");
            bind(L, constructor, KEY_RIGHTPAREN, "RightParen");
            bind(L, constructor, KEY_ASTERISK, "Asterisk");
            bind(L, constructor, KEY_PLUS, "Plus");
            bind(L, constructor, KEY_COMMA, "Comma");
            bind(L, constructor, KEY_MINUS, "Minus");
            bind(L, constructor, KEY_PERIOD, "Period");
            bind(L, constructor, KEY_SLASH, "Slash");
            bind(L, constructor, KEY_0, "0");
            bind(L, constructor, KEY_1, "1");
            bind(L, constructor, KEY_2, "2");
            bind(L, constructor, KEY_3, "3");
            bind(L, constructor, KEY_4, "4");
            bind(L, constructor, KEY_5, "5");
            bind(L, constructor, KEY_6, "6");
            bind(L, constructor, KEY_7, "7");
            bind(L, constructor, KEY_8, "8");
            bind(L, constructor, KEY_9, "9");
            bind(L, constructor, KEY_COLON, "Colon");
            bind(L, constructor, KEY_SEMICOLON, "Semicolon");
            bind(L, constructor, KEY_LESSTHAN, "LessThan");
            bind(L, constructor, KEY_EQUALS, "Equals");
            bind(L, constructor, KEY_GREATERTHAN, "GreaterThan");
            bind(L, constructor, KEY_QUESTION, "Question");
            bind(L, constructor, KEY_AT, "At");
            bind(L, constructor, KEY_LEFTBRACKET, "LeftBracket");
            bind(L, constructor, KEY_BACKSLASH, "Backslash");
            bind(L, constructor, KEY_RIGHTBRACKET, "RightBracket");
            bind(L, constructor, KEY_CARET, "Caret");
            bind(L, constructor, KEY_UNDERSCORE, "Underscore");
            bind(L, constructor, KEY_TILDE, "Tilde");
            bind(L, constructor, KEY_BACKQUOTE, "Backquote");
            bind(L, constructor, KEY_GRAVE, "Grave");
            bind(L, constructor, KEY_A, "A");
            bind(L, constructor, KEY_B, "B");
            bind(L, constructor, KEY_C, "C");
            bind(L, constructor, KEY_D, "D");
            bind(L, constructor, KEY_E, "E");
            bind(L, constructor, KEY_F, "F");
            bind(L, constructor, KEY_G, "G");
            bind(L, constructor, KEY_H, "H");
            bind(L, constructor, KEY_I, "I");
            bind(L, constructor, KEY_J, "J");
            bind(L, constructor, KEY_K, "K");
            bind(L, constructor, KEY_L, "L");
            bind(L, constructor, KEY_M, "M");
            bind(L, constructor, KEY_N, "N");
            bind(L, constructor, KEY_O, "O");
            bind(L, constructor, KEY_P, "P");
            bind(L, constructor, KEY_Q, "Q");
            bind(L, constructor, KEY_R, "R");
            bind(L, constructor, KEY_S, "S");
            bind(L, constructor, KEY_T, "T");
            bind(L, constructor, KEY_U, "U");
            bind(L, constructor, KEY_V, "V");
            bind(L, constructor, KEY_W, "W");
            bind(L, constructor, KEY_X, "X");
            bind(L, constructor, KEY_Y, "Y");
            bind(L, constructor, KEY_Z, "Z");
            bind(L, constructor, KEY_DELETE, "Delete");
            bind(L, constructor, KEY_NUMPAD_0, "Numpad0");
            bind(L, constructor, KEY_NUMPAD_1, "Numpad1");
            bind(L, constructor, KEY_NUMPAD_2, "Numpad2");
            bind(L, constructor, KEY_NUMPAD_3, "Numpad3");
            bind(L, constructor, KEY_NUMPAD_4, "Numpad4");
            bind(L, constructor, KEY_NUMPAD_5, "Numpad5");
            bind(L, constructor, KEY_NUMPAD_6, "Numpad6");
            bind(L, constructor, KEY_NUMPAD_7, "Numpad7");
            bind(L, constructor, KEY_NUMPAD_8, "Numpad8");
            bind(L, constructor, KEY_NUMPAD_9, "Numpad9");
            bind(L, constructor, KEY_NUMPAD_PERIOD, "NumpadPeriod");
            bind(L, constructor, KEY_NUMPAD_DIVIDE, "NumpadDivide");
            bind(L, constructor, KEY_NUMPAD_MULTIPLY, "NumpadMultiply");
            bind(L, constructor, KEY_NUMPAD_MINUS, "NumpadMinus");
            bind(L, constructor, KEY_NUMPAD_PLUS, "NumpadPlus");
            bind(L, constructor, KEY_NUMPAD_ENTER, "NumpadEnter");
            bind(L, constructor, KEY_NUMPAD_EQUALS, "NumpadEquals");
            bind(L, constructor, KEY_UP, "Up");
            bind(L, constructor, KEY_DOWN, "Down");
            bind(L, constructor, KEY_LEFT, "Left");
            bind(L, constructor, KEY_RIGHT, "Right");
            bind(L, constructor, KEY_INSERT, "Insert");
            bind(L, constructor, KEY_HOME, "Home");
            bind(L, constructor, KEY_END, "End");
            bind(L, constructor, KEY_PAGEUP, "PageUp");
            bind(L, constructor, KEY_PAGEDOWN, "PageDown");
            bind(L, constructor, KEY_F1, "F1");
            bind(L, constructor, KEY_F2, "F2");
            bind(L, constructor, KEY_F3, "F3");
            bind(L, constructor, KEY_F4, "F4");
            bind(L, constructor, KEY_F5, "F5");
            bind(L, constructor, KEY_F6, "F6");
            bind(L, constructor, KEY_F7, "F7");
            bind(L, constructor, KEY_F8, "F8");
            bind(L, constructor, KEY_F9, "F9");
            bind(L, constructor, KEY_F10, "F10");
            bind(L, constructor, KEY_F11, "F11");
            bind(L, constructor, KEY_F12, "F12");
            bind(L, constructor, KEY_NUMLOCK, "NumLock");
            bind(L, constructor, KEY_CAPSLOCK, "CapsLock");
            bind(L, constructor, KEY_SCROLLOCK, "ScrollLock");
            bind(L, constructor, KEY_RSHIFT, "RightShift");
            bind(L, constructor, KEY_LSHIFT, "LeftShift");
            bind(L, constructor, KEY_RCTRL, "RightCtrl");
            bind(L, constructor, KEY_LCTRL, "LeftCtrl");
            bind(L, constructor, KEY_RALT, "RightAlt");
            bind(L, constructor, KEY_LALT, "LeftAlt");
            bind(L, constructor, KEY_BREAK, "Break");

            // Pop plum.key table. (Whew.)
            lua_pop(L, 1);
    
            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}