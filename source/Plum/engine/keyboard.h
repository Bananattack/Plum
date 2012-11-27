#pragma once

namespace plum
{
    const int KEY_MAX_COUNT = SDLK_LAST;

    class KeyInput : public Input
    {
        private:
            bool pressed;
        public:
            KeyInput() { pressed = false; }
            virtual bool isPressed() { return pressed; }
            virtual void setPressed(bool pressed) { this->pressed = pressed; }
    };

    // Key defines, these go on a while...
    const int KEY_BACKSPACE = SDLK_BACKSPACE;
    const int KEY_TAB = SDLK_TAB;
    const int KEY_ENTER = SDLK_RETURN;
    const int KEY_PAUSE = SDLK_PAUSE;
    const int KEY_ESCAPE = SDLK_ESCAPE;
    const int KEY_SPACE = SDLK_SPACE;
    const int KEY_EXCLAIM = SDLK_EXCLAIM;
    const int KEY_QUOTE = SDLK_QUOTEDBL;
    const int KEY_HASH = SDLK_HASH;
    const int KEY_DOLLAR = SDLK_DOLLAR;
    const int KEY_AMPERSAND = SDLK_AMPERSAND;
    const int KEY_APOSTROPHE = SDLK_QUOTE;
    const int KEY_LEFTPAREN = SDLK_LEFTPAREN;
    const int KEY_RIGHTPAREN = SDLK_RIGHTPAREN;
    const int KEY_ASTERISK = SDLK_ASTERISK;
    const int KEY_PLUS = SDLK_PLUS;
    const int KEY_COMMA = SDLK_COMMA;
    const int KEY_MINUS = SDLK_MINUS;
    const int KEY_PERIOD = SDLK_PERIOD;
    const int KEY_SLASH = SDLK_SLASH;
    const int KEY_0 = SDLK_0;
    const int KEY_1 = SDLK_1;
    const int KEY_2 = SDLK_2;
    const int KEY_3 = SDLK_3;
    const int KEY_4 = SDLK_4;
    const int KEY_5 = SDLK_5;
    const int KEY_6 = SDLK_6;
    const int KEY_7 = SDLK_7;
    const int KEY_8 = SDLK_8;
    const int KEY_9 = SDLK_9;
    const int KEY_COLON = SDLK_COLON;
    const int KEY_SEMICOLON = SDLK_SEMICOLON;
    const int KEY_LESSTHAN = SDLK_LESS;
    const int KEY_EQUALS = SDLK_EQUALS;
    const int KEY_GREATERTHAN = SDLK_GREATER;
    const int KEY_QUESTION = SDLK_QUESTION;
    const int KEY_AT = SDLK_AT;
    const int KEY_LEFTBRACKET = SDLK_LEFTBRACKET;
    const int KEY_BACKSLASH = SDLK_BACKSLASH;
    const int KEY_RIGHTBRACKET = SDLK_RIGHTBRACKET;
    const int KEY_CARET = SDLK_CARET;
    const int KEY_UNDERSCORE = SDLK_UNDERSCORE;
    const int KEY_TILDE = SDLK_BACKQUOTE;
    const int KEY_BACKQUOTE = SDLK_BACKQUOTE;
    const int KEY_GRAVE = SDLK_BACKQUOTE;
    const int KEY_A = SDLK_a;
    const int KEY_B = SDLK_b;
    const int KEY_C = SDLK_c;
    const int KEY_D = SDLK_d;
    const int KEY_E = SDLK_e;
    const int KEY_F = SDLK_f;
    const int KEY_G = SDLK_g;
    const int KEY_H = SDLK_h;
    const int KEY_I = SDLK_i;
    const int KEY_J = SDLK_j;
    const int KEY_K = SDLK_k;
    const int KEY_L = SDLK_l;
    const int KEY_M = SDLK_m;
    const int KEY_N = SDLK_n;
    const int KEY_O = SDLK_o;
    const int KEY_P = SDLK_p;
    const int KEY_Q = SDLK_q;
    const int KEY_R = SDLK_r;
    const int KEY_S = SDLK_s;
    const int KEY_T = SDLK_t;
    const int KEY_U = SDLK_u;
    const int KEY_V = SDLK_v;
    const int KEY_W = SDLK_w;
    const int KEY_X = SDLK_x;
    const int KEY_Y = SDLK_y;
    const int KEY_Z = SDLK_z;
    const int KEY_DELETE = SDLK_DELETE;
    const int KEY_NUMPAD_0 = SDLK_KP0;
    const int KEY_NUMPAD_1 = SDLK_KP1;
    const int KEY_NUMPAD_2 = SDLK_KP2;
    const int KEY_NUMPAD_3 = SDLK_KP3;
    const int KEY_NUMPAD_4 = SDLK_KP4;
    const int KEY_NUMPAD_5 = SDLK_KP5;
    const int KEY_NUMPAD_6 = SDLK_KP6;
    const int KEY_NUMPAD_7 = SDLK_KP7;
    const int KEY_NUMPAD_8 = SDLK_KP8;
    const int KEY_NUMPAD_9 = SDLK_KP9;
    const int KEY_NUMPAD_PERIOD = SDLK_KP_PERIOD;
    const int KEY_NUMPAD_DIVIDE = SDLK_KP_DIVIDE;
    const int KEY_NUMPAD_MULTIPLY = SDLK_KP_MULTIPLY;
    const int KEY_NUMPAD_MINUS = SDLK_KP_MINUS;
    const int KEY_NUMPAD_PLUS = SDLK_KP_PLUS;
    const int KEY_NUMPAD_ENTER = SDLK_KP_ENTER;
    const int KEY_NUMPAD_EQUALS = SDLK_KP_EQUALS;
    const int KEY_UP = SDLK_UP;
    const int KEY_DOWN = SDLK_DOWN;
    const int KEY_LEFT = SDLK_LEFT;
    const int KEY_RIGHT = SDLK_RIGHT;
    const int KEY_INSERT = SDLK_INSERT;
    const int KEY_HOME = SDLK_HOME;
    const int KEY_END = SDLK_END;
    const int KEY_PAGEUP = SDLK_PAGEUP;
    const int KEY_PAGEDOWN = SDLK_PAGEDOWN;
    const int KEY_F1 = SDLK_F1;
    const int KEY_F2 = SDLK_F2;
    const int KEY_F3 = SDLK_F3;
    const int KEY_F4 = SDLK_F4;
    const int KEY_F5 = SDLK_F5;
    const int KEY_F6 = SDLK_F6;
    const int KEY_F7 = SDLK_F7;
    const int KEY_F8 = SDLK_F8;
    const int KEY_F9 = SDLK_F9;
    const int KEY_F10 = SDLK_F10;
    const int KEY_F11 = SDLK_F11;
    const int KEY_F12 = SDLK_F12;
    const int KEY_NUMLOCK = SDLK_NUMLOCK;
    const int KEY_CAPSLOCK = SDLK_CAPSLOCK;
    const int KEY_SCROLLOCK = SDLK_SCROLLOCK;
    const int KEY_SCROLLLOCK = SDLK_SCROLLOCK;
    const int KEY_RSHIFT = SDLK_RSHIFT;
    const int KEY_LSHIFT = SDLK_LSHIFT;
    const int KEY_RCTRL = SDLK_RCTRL;
    const int KEY_LCTRL = SDLK_LCTRL;
    const int KEY_RALT = SDLK_RALT;
    const int KEY_LALT = SDLK_LALT;
    const int KEY_BREAK = SDLK_BREAK;
}