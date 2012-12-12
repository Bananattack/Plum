

#include "../core/input.h"
#include "script.h"

namespace plum
{
    namespace
    {
        void bind(lua_State* L, lua_CFunction constructor, int index, const char* name)
        {
            // plum.key[index] = plum.Input(keyref);
            lua_pushcfunction(L, constructor);
            lua_pushlightuserdata(L, &(script::instance(L).keyboard()[Key(index)]));
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
            script::push(L, "key");
            lua_pushvalue(L, -2);
            lua_settable(L, -4);

            // Oh boy, here comes the the fun part.
            bind(L, constructor, KeyBackspace, "Backspace");
            bind(L, constructor, KeyTab, "Tab");
            bind(L, constructor, KeyEnter, "Enter");
            bind(L, constructor, KeyPause, "Pause");
            bind(L, constructor, KeyEscape, "Escape");
            bind(L, constructor, KeySpace, "Space");
            bind(L, constructor, KeyExclaim, "Exclaim");
            bind(L, constructor, KeyQuote, "Quote");
            bind(L, constructor, KeyHash, "Hash");
            bind(L, constructor, KeyDollar, "Dollar");
            bind(L, constructor, KeyAmpersand, "Ampersand");
            bind(L, constructor, KeyApostrophe, "Apostrophe");
            bind(L, constructor, KeyLeftParen, "LeftParen");
            bind(L, constructor, KeyRightParen, "RightParen");
            bind(L, constructor, KeyAsterisk, "Asterisk");
            bind(L, constructor, KeyPlus, "Plus");
            bind(L, constructor, KeyComma, "Comma");
            bind(L, constructor, KeyMinus, "Minus");
            bind(L, constructor, KeyPeriod, "Period");
            bind(L, constructor, KeySlash, "Slash");
            bind(L, constructor, Key0, "0");
            bind(L, constructor, Key1, "1");
            bind(L, constructor, Key2, "2");
            bind(L, constructor, Key3, "3");
            bind(L, constructor, Key4, "4");
            bind(L, constructor, Key5, "5");
            bind(L, constructor, Key6, "6");
            bind(L, constructor, Key7, "7");
            bind(L, constructor, Key8, "8");
            bind(L, constructor, Key9, "9");
            bind(L, constructor, KeyColon, "Colon");
            bind(L, constructor, KeySemiColon, "Semicolon");
            bind(L, constructor, KeyLessThan, "LessThan");
            bind(L, constructor, KeyEquals, "Equals");
            bind(L, constructor, KeyGreaterThan, "GreaterThan");
            bind(L, constructor, KeyQuestion, "Question");
            bind(L, constructor, KeyAt, "At");
            bind(L, constructor, KeyLeftBracket, "LeftBracket");
            bind(L, constructor, KeyBackslash, "Backslash");
            bind(L, constructor, KeyRightBracket, "RightBracket");
            bind(L, constructor, KeyCaret, "Caret");
            bind(L, constructor, KeyUnderscore, "Underscore");
            bind(L, constructor, KeyTilde, "Tilde");
            bind(L, constructor, KeyBackquote, "Backquote");
            bind(L, constructor, KeyGrave, "Grave");
            bind(L, constructor, KeyA, "A");
            bind(L, constructor, KeyB, "B");
            bind(L, constructor, KeyC, "C");
            bind(L, constructor, KeyD, "D");
            bind(L, constructor, KeyE, "E");
            bind(L, constructor, KeyF, "F");
            bind(L, constructor, KeyG, "G");
            bind(L, constructor, KeyH, "H");
            bind(L, constructor, KeyI, "I");
            bind(L, constructor, KeyJ, "J");
            bind(L, constructor, KeyK, "K");
            bind(L, constructor, KeyL, "L");
            bind(L, constructor, KeyM, "M");
            bind(L, constructor, KeyN, "N");
            bind(L, constructor, KeyO, "O");
            bind(L, constructor, KeyP, "P");
            bind(L, constructor, KeyQ, "Q");
            bind(L, constructor, KeyR, "R");
            bind(L, constructor, KeyS, "S");
            bind(L, constructor, KeyT, "T");
            bind(L, constructor, KeyU, "U");
            bind(L, constructor, KeyV, "V");
            bind(L, constructor, KeyW, "W");
            bind(L, constructor, KeyX, "X");
            bind(L, constructor, KeyY, "Y");
            bind(L, constructor, KeyZ, "Z");
            bind(L, constructor, KeyDelete, "Delete");
            bind(L, constructor, KeyNumpad0, "Numpad0");
            bind(L, constructor, KeyNumpad1, "Numpad1");
            bind(L, constructor, KeyNumpad2, "Numpad2");
            bind(L, constructor, KeyNumpad3, "Numpad3");
            bind(L, constructor, KeyNumpad4, "Numpad4");
            bind(L, constructor, KeyNumpad5, "Numpad5");
            bind(L, constructor, KeyNumpad6, "Numpad6");
            bind(L, constructor, KeyNumpad7, "Numpad7");
            bind(L, constructor, KeyNumpad8, "Numpad8");
            bind(L, constructor, KeyNumpad9, "Numpad9");
            bind(L, constructor, KeyNumpadPeriod, "NumpadPeriod");
            bind(L, constructor, KeyNumpadDivide, "NumpadDivide");
            bind(L, constructor, KeyNumpadMultiply, "NumpadMultiply");
            bind(L, constructor, KeyNumpadMinus, "NumpadMinus");
            bind(L, constructor, KeyNumpadPlus, "NumpadPlus");
            bind(L, constructor, KeyNumpadEnter, "NumpadEnter");
            bind(L, constructor, KeyNumpadEquals, "NumpadEquals");
            bind(L, constructor, KeyUp, "Up");
            bind(L, constructor, KeyDown, "Down");
            bind(L, constructor, KeyLeft, "Left");
            bind(L, constructor, KeyRight, "Right");
            bind(L, constructor, KeyInsert, "Insert");
            bind(L, constructor, KeyHome, "Home");
            bind(L, constructor, KeyEnd, "End");
            bind(L, constructor, KeyPageUp, "PageUp");
            bind(L, constructor, KeyPageDown, "PageDown");
            bind(L, constructor, KeyF1, "F1");
            bind(L, constructor, KeyF2, "F2");
            bind(L, constructor, KeyF3, "F3");
            bind(L, constructor, KeyF4, "F4");
            bind(L, constructor, KeyF5, "F5");
            bind(L, constructor, KeyF6, "F6");
            bind(L, constructor, KeyF7, "F7");
            bind(L, constructor, KeyF8, "F8");
            bind(L, constructor, KeyF9, "F9");
            bind(L, constructor, KeyF10, "F10");
            bind(L, constructor, KeyF11, "F11");
            bind(L, constructor, KeyF12, "F12");
            bind(L, constructor, KeyNumLock, "NumLock");
            bind(L, constructor, KeyCapsLock, "CapsLock");
            bind(L, constructor, KeyScrollLock, "ScrollLock");
            bind(L, constructor, KeyLeftShift, "LeftShift");
            bind(L, constructor, KeyRightShift, "RightShift");
            bind(L, constructor, KeyLeftCtrl, "LeftCtrl");
            bind(L, constructor, KeyRightCtrl, "RightCtrl");
            bind(L, constructor, KeyLeftAlt, "LeftAlt");
            bind(L, constructor, KeyRightAlt, "RightAlt");
            bind(L, constructor, KeyBreak, "Break");

            // Pop plum.key table. (Whew.)
            lua_pop(L, 1);
    
            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}