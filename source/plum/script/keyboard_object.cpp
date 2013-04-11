

#include "../core/input.h"
#include "script.h"

namespace plum
{
    namespace
    {
        void bind(lua_State* L, Keyboard& key, Key code, const char* name)
        {
            // Designate the parent of this keyboard input.
            lua_pushvalue(L, -2);
            int ref = luaL_ref(L, LUA_REGISTRYINDEX);
            script::push(L, &key[code], ref);
            // key[name] = <ref to C-side key[code] object>
            lua_setfield(L, -2, name);
        }
    }

    namespace script
    {
        void pushKeyboardTable(lua_State* L, Keyboard& key)
        {
            // Assumes parent value is at the top of the stack
            // which this uses as the owner of the keyboard attributes.
            // t = {}
            lua_newtable(L);

            // Oh boy, here comes the the fun part.
            bind(L, key, KeyBackspace, "Backspace");
            bind(L, key, KeyTab, "Tab");
            bind(L, key, KeyEnter, "Enter");
            bind(L, key, KeyPause, "Pause");
            bind(L, key, KeyEscape, "Escape");
            bind(L, key, KeySpace, "Space");
            bind(L, key, KeyExclaim, "Exclaim");
            bind(L, key, KeyQuote, "Quote");
            bind(L, key, KeyHash, "Hash");
            bind(L, key, KeyDollar, "Dollar");
            bind(L, key, KeyAmpersand, "Ampersand");
            bind(L, key, KeyApostrophe, "Apostrophe");
            bind(L, key, KeyLeftParen, "LeftParen");
            bind(L, key, KeyRightParen, "RightParen");
            bind(L, key, KeyAsterisk, "Asterisk");
            bind(L, key, KeyPlus, "Plus");
            bind(L, key, KeyComma, "Comma");
            bind(L, key, KeyMinus, "Minus");
            bind(L, key, KeyPeriod, "Period");
            bind(L, key, KeySlash, "Slash");
            bind(L, key, Key0, "0");
            bind(L, key, Key1, "1");
            bind(L, key, Key2, "2");
            bind(L, key, Key3, "3");
            bind(L, key, Key4, "4");
            bind(L, key, Key5, "5");
            bind(L, key, Key6, "6");
            bind(L, key, Key7, "7");
            bind(L, key, Key8, "8");
            bind(L, key, Key9, "9");
            bind(L, key, KeyColon, "Colon");
            bind(L, key, KeySemiColon, "Semicolon");
            bind(L, key, KeyLessThan, "LessThan");
            bind(L, key, KeyEquals, "Equals");
            bind(L, key, KeyGreaterThan, "GreaterThan");
            bind(L, key, KeyQuestion, "Question");
            bind(L, key, KeyAt, "At");
            bind(L, key, KeyLeftBracket, "LeftBracket");
            bind(L, key, KeyBackslash, "Backslash");
            bind(L, key, KeyRightBracket, "RightBracket");
            bind(L, key, KeyCaret, "Caret");
            bind(L, key, KeyUnderscore, "Underscore");
            bind(L, key, KeyTilde, "Tilde");
            bind(L, key, KeyBackquote, "Backquote");
            bind(L, key, KeyGrave, "Grave");
            bind(L, key, KeyA, "A");
            bind(L, key, KeyB, "B");
            bind(L, key, KeyC, "C");
            bind(L, key, KeyD, "D");
            bind(L, key, KeyE, "E");
            bind(L, key, KeyF, "F");
            bind(L, key, KeyG, "G");
            bind(L, key, KeyH, "H");
            bind(L, key, KeyI, "I");
            bind(L, key, KeyJ, "J");
            bind(L, key, KeyK, "K");
            bind(L, key, KeyL, "L");
            bind(L, key, KeyM, "M");
            bind(L, key, KeyN, "N");
            bind(L, key, KeyO, "O");
            bind(L, key, KeyP, "P");
            bind(L, key, KeyQ, "Q");
            bind(L, key, KeyR, "R");
            bind(L, key, KeyS, "S");
            bind(L, key, KeyT, "T");
            bind(L, key, KeyU, "U");
            bind(L, key, KeyV, "V");
            bind(L, key, KeyW, "W");
            bind(L, key, KeyX, "X");
            bind(L, key, KeyY, "Y");
            bind(L, key, KeyZ, "Z");
            bind(L, key, KeyDelete, "Delete");
            bind(L, key, KeyNumpad0, "Numpad0");
            bind(L, key, KeyNumpad1, "Numpad1");
            bind(L, key, KeyNumpad2, "Numpad2");
            bind(L, key, KeyNumpad3, "Numpad3");
            bind(L, key, KeyNumpad4, "Numpad4");
            bind(L, key, KeyNumpad5, "Numpad5");
            bind(L, key, KeyNumpad6, "Numpad6");
            bind(L, key, KeyNumpad7, "Numpad7");
            bind(L, key, KeyNumpad8, "Numpad8");
            bind(L, key, KeyNumpad9, "Numpad9");
            bind(L, key, KeyNumpadPeriod, "NumpadPeriod");
            bind(L, key, KeyNumpadDivide, "NumpadDivide");
            bind(L, key, KeyNumpadMultiply, "NumpadMultiply");
            bind(L, key, KeyNumpadMinus, "NumpadMinus");
            bind(L, key, KeyNumpadPlus, "NumpadPlus");
            bind(L, key, KeyNumpadEnter, "NumpadEnter");
            bind(L, key, KeyNumpadEquals, "NumpadEquals");
            bind(L, key, KeyUp, "Up");
            bind(L, key, KeyDown, "Down");
            bind(L, key, KeyLeft, "Left");
            bind(L, key, KeyRight, "Right");
            bind(L, key, KeyInsert, "Insert");
            bind(L, key, KeyHome, "Home");
            bind(L, key, KeyEnd, "End");
            bind(L, key, KeyPageUp, "PageUp");
            bind(L, key, KeyPageDown, "PageDown");
            bind(L, key, KeyF1, "F1");
            bind(L, key, KeyF2, "F2");
            bind(L, key, KeyF3, "F3");
            bind(L, key, KeyF4, "F4");
            bind(L, key, KeyF5, "F5");
            bind(L, key, KeyF6, "F6");
            bind(L, key, KeyF7, "F7");
            bind(L, key, KeyF8, "F8");
            bind(L, key, KeyF9, "F9");
            bind(L, key, KeyF10, "F10");
            bind(L, key, KeyF11, "F11");
            bind(L, key, KeyF12, "F12");
            bind(L, key, KeyNumLock, "NumLock");
            bind(L, key, KeyCapsLock, "CapsLock");
            bind(L, key, KeyScrollLock, "ScrollLock");
            bind(L, key, KeyLeftShift, "LeftShift");
            bind(L, key, KeyRightShift, "RightShift");
            bind(L, key, KeyLeftCtrl, "LeftCtrl");
            bind(L, key, KeyRightCtrl, "RightCtrl");
            bind(L, key, KeyLeftAlt, "LeftAlt");
            bind(L, key, KeyRightAlt, "RightAlt");
            bind(L, key, KeyBreak, "Break");
        }
    }
}