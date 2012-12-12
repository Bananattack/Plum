#include <functional>
#include <SDL.h>

#include "event.h"
#include "../../core/input.h"
#include "../../core/engine.h"

namespace plum    
{
    namespace
    {
        SDLKey getSDLKey(Key k)
        {
            switch(k)
            {
                case KeyBackspace: return SDLK_BACKSPACE;
                case KeyTab: return SDLK_TAB;
                case KeyEnter: return SDLK_RETURN;
                case KeyPause: return SDLK_PAUSE;
                case KeyEscape: return SDLK_ESCAPE;
                case KeySpace: return SDLK_SPACE;
                case KeyExclaim: return SDLK_EXCLAIM;
                case KeyQuote: return SDLK_QUOTEDBL;
                case KeyHash: return SDLK_HASH;
                case KeyDollar: return SDLK_DOLLAR;
                case KeyAmpersand: return SDLK_AMPERSAND;
                case KeyApostrophe: return SDLK_QUOTE;
                case KeyLeftParen: return SDLK_LEFTPAREN;
                case KeyRightParen: return SDLK_RIGHTPAREN;
                case KeyAsterisk: return SDLK_ASTERISK;
                case KeyPlus: return SDLK_PLUS;
                case KeyComma: return SDLK_COMMA;
                case KeyMinus: return SDLK_MINUS;
                case KeyPeriod: return SDLK_PERIOD;
                case KeySlash: return SDLK_SLASH;
                case Key0: return SDLK_0;
                case Key1: return SDLK_1;
                case Key2: return SDLK_2;
                case Key3: return SDLK_3;
                case Key4: return SDLK_4;
                case Key5: return SDLK_5;
                case Key6: return SDLK_6;
                case Key7: return SDLK_7;
                case Key8: return SDLK_8;
                case Key9: return SDLK_9;
                case KeyColon: return SDLK_COLON;
                case KeySemiColon: return SDLK_SEMICOLON;
                case KeyLessThan: return SDLK_LESS;
                case KeyEquals: return SDLK_EQUALS;
                case KeyGreaterThan: return SDLK_GREATER;
                case KeyQuestion: return SDLK_QUESTION;
                case KeyAt: return SDLK_AT;
                case KeyLeftBracket: return SDLK_LEFTBRACKET;
                case KeyBackslash: return SDLK_BACKSLASH;
                case KeyRightBracket: return SDLK_RIGHTBRACKET;
                case KeyCaret: return SDLK_CARET;
                case KeyUnderscore: return SDLK_UNDERSCORE;
                case KeyTilde: return SDLK_BACKQUOTE;
                case KeyBackquote: return SDLK_BACKQUOTE;
                case KeyGrave: return SDLK_BACKQUOTE;
                case KeyA: return SDLK_a;
                case KeyB: return SDLK_b;
                case KeyC: return SDLK_c;
                case KeyD: return SDLK_d;
                case KeyE: return SDLK_e;
                case KeyF: return SDLK_f;
                case KeyG: return SDLK_g;
                case KeyH: return SDLK_h;
                case KeyI: return SDLK_i;
                case KeyJ: return SDLK_j;
                case KeyK: return SDLK_k;
                case KeyL: return SDLK_l;
                case KeyM: return SDLK_m;
                case KeyN: return SDLK_n;
                case KeyO: return SDLK_o;
                case KeyP: return SDLK_p;
                case KeyQ: return SDLK_q;
                case KeyR: return SDLK_r;
                case KeyS: return SDLK_s;
                case KeyT: return SDLK_t;
                case KeyU: return SDLK_u;
                case KeyV: return SDLK_v;
                case KeyW: return SDLK_w;
                case KeyX: return SDLK_x;
                case KeyY: return SDLK_y;
                case KeyZ: return SDLK_z;
                case KeyDelete: return SDLK_DELETE;
                case KeyNumpad0: return SDLK_KP0;
                case KeyNumpad1: return SDLK_KP1;
                case KeyNumpad2: return SDLK_KP2;
                case KeyNumpad3: return SDLK_KP3;
                case KeyNumpad4: return SDLK_KP4;
                case KeyNumpad5: return SDLK_KP5;
                case KeyNumpad6: return SDLK_KP6;
                case KeyNumpad7: return SDLK_KP7;
                case KeyNumpad8: return SDLK_KP8;
                case KeyNumpad9: return SDLK_KP9;
                case KeyNumpadPeriod: return SDLK_KP_PERIOD;
                case KeyNumpadDivide: return SDLK_KP_DIVIDE;
                case KeyNumpadMultiply: return SDLK_KP_MULTIPLY;
                case KeyNumpadMinus: return SDLK_KP_MINUS;
                case KeyNumpadPlus: return SDLK_KP_PLUS;
                case KeyNumpadEnter: return SDLK_KP_ENTER;
                case KeyNumpadEquals: return SDLK_KP_EQUALS;
                case KeyUp: return SDLK_UP;
                case KeyDown: return SDLK_DOWN;
                case KeyLeft: return SDLK_LEFT;
                case KeyRight: return SDLK_RIGHT;
                case KeyInsert: return SDLK_INSERT;
                case KeyHome: return SDLK_HOME;
                case KeyEnd: return SDLK_END;
                case KeyPageUp: return SDLK_PAGEUP;
                case KeyPageDown: return SDLK_PAGEDOWN;
                case KeyF1: return SDLK_F1;
                case KeyF2: return SDLK_F2;
                case KeyF3: return SDLK_F3;
                case KeyF4: return SDLK_F4;
                case KeyF5: return SDLK_F5;
                case KeyF6: return SDLK_F6;
                case KeyF7: return SDLK_F7;
                case KeyF8: return SDLK_F8;
                case KeyF9: return SDLK_F9;
                case KeyF10: return SDLK_F10;
                case KeyF11: return SDLK_F11;
                case KeyF12: return SDLK_F12;
                case KeyNumLock: return SDLK_NUMLOCK;
                case KeyCapsLock: return SDLK_CAPSLOCK;
                case KeyScrollLock: return SDLK_SCROLLOCK;
                case KeyLeftShift: return SDLK_LSHIFT;
                case KeyRightShift: return SDLK_RSHIFT;
                case KeyLeftCtrl: return SDLK_LCTRL;
                case KeyRightCtrl: return SDLK_RCTRL;
                case KeyLeftAlt: return SDLK_LALT;
                case KeyRightAlt: return SDLK_RALT;
                case KeyBreak: return SDLK_BREAK;
                default: return SDLK_UNKNOWN;
            }
        }
    }



    class Keyboard::Impl
    {
        public:
            Impl(Engine& engine)
                : engine(engine)
            {
                hook = engine.addEventHook([this](const Event& event){ handle(event); });
            }

            ~Impl()
            {
            }

            void handle(const Event& event)
            {
                const auto& e(event.e);
                switch(e.type)
                {
                    case SDL_KEYDOWN:
                    case SDL_KEYUP:
                        keys[e.key.keysym.sym].setPressed(e.type == SDL_KEYDOWN);
                        break;
                    default: break;
                }
            }
            

            Engine& engine;
            std::shared_ptr<Engine::EventHook> hook;
            Input keys[SDLK_LAST];
    };

    Keyboard::Keyboard(Engine& engine)
        : impl(new Impl(engine))
    {
    }

    Keyboard::~Keyboard()
    {
    }

    Input& Keyboard::operator[](Key k)
    {
        return impl->keys[getSDLKey(k)];
    }



    class Mouse::Impl
    {
        public:
            Impl(Engine& engine)
                : engine(engine)
            {
                hook = engine.addEventHook([this](const Event& event){ handle(event); });
            }

            ~Impl()
            {
            }

            void handle(const Event& event)
            {
                const auto& e(event.e);
                switch(e.type)
                {
                    case SDL_MOUSEBUTTONDOWN:
                    case SDL_MOUSEBUTTONUP:
                    {
                        bool pressed = e.type == SDL_MOUSEBUTTONDOWN;
                        switch(e.button.type)
                        {
                            case SDL_BUTTON_LEFT: l.setPressed(pressed); break;
                            case SDL_BUTTON_MIDDLE: m.setPressed(pressed); break;
                            case SDL_BUTTON_RIGHT: m.setPressed(pressed); break;
                            case SDL_BUTTON_WHEELUP: wu.setPressed(pressed); break;
                            case SDL_BUTTON_WHEELDOWN: wd.setPressed(pressed); break;
                            default: break;
                        }
                        break;
                    }
                    case SDL_MOUSEMOTION:
                        x = e.motion.x;
                        y = e.motion.y;
                        break;
                    default: break;
                }
            }

            Engine& engine;
            std::shared_ptr<Engine::EventHook> hook;
            Input l, m, r, wu, wd;
            double x, y;
    };

    Mouse::Mouse(Engine& engine)
        : impl(new Impl(engine))
    {
    }

    Mouse::~Mouse()
    {
    }

    Input& Mouse::getLeft()
    {
        return impl->l;
    }

    Input& Mouse::getMiddle()
    {
        return impl->m;
    }

    Input& Mouse::getRight()
    {
        return impl->r;
    }

    Input& Mouse::getWheelUp()
    {
        return impl->wu;
    }

    Input& Mouse::getWheelDown()
    {
        return impl->wd;
    }

    double Mouse::getX()
    {
        return impl->x;
    }

    double Mouse::getY()
    {
        return impl->y;
    }
}