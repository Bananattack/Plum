#include <functional>
#include <cstring>

#include "engine.h"
#include "../../core/screen.h"
#include "../../core/input.h"

namespace plum    
{
    namespace
    {
        int getGLFWKey(Key k)
        {
            switch(k)
            {
                case KeyBackspace: return GLFW_KEY_BACKSPACE;
                case KeyTab: return GLFW_KEY_TAB;
                case KeyEnter: return GLFW_KEY_ENTER;
                case KeyPause: return GLFW_KEY_PAUSE;
                case KeyEscape: return GLFW_KEY_ESCAPE;
                case KeySpace: return GLFW_KEY_SPACE;
                case KeyApostrophe: return GLFW_KEY_APOSTROPHE;
                case KeyComma: return GLFW_KEY_COMMA;
                case KeyMinus: return GLFW_KEY_MINUS;
                case KeyPeriod: return GLFW_KEY_PERIOD;
                case KeySlash: return GLFW_KEY_SLASH;
                case Key0: return GLFW_KEY_0;
                case Key1: return GLFW_KEY_1;
                case Key2: return GLFW_KEY_2;
                case Key3: return GLFW_KEY_3;
                case Key4: return GLFW_KEY_4;
                case Key5: return GLFW_KEY_5;
                case Key6: return GLFW_KEY_6;
                case Key7: return GLFW_KEY_7;
                case Key8: return GLFW_KEY_8;
                case Key9: return GLFW_KEY_9;
                case KeySemiColon: return GLFW_KEY_SEMICOLON;
                case KeyEquals: return GLFW_KEY_EQUAL;
                case KeyLeftBracket: return GLFW_KEY_LEFT_BRACKET;
                case KeyBackslash: return GLFW_KEY_BACKSLASH;
                case KeyRightBracket: return GLFW_KEY_RIGHT_BRACKET;
                case KeyTilde: return GLFW_KEY_GRAVE_ACCENT;
                case KeyBackquote: return GLFW_KEY_GRAVE_ACCENT;
                case KeyGrave: return GLFW_KEY_GRAVE_ACCENT;
                case KeyA: return GLFW_KEY_A;
                case KeyB: return GLFW_KEY_B;
                case KeyC: return GLFW_KEY_C;
                case KeyD: return GLFW_KEY_D;
                case KeyE: return GLFW_KEY_E;
                case KeyF: return GLFW_KEY_F;
                case KeyR: return GLFW_KEY_R;
                case KeyG: return GLFW_KEY_G;
                case KeyH: return GLFW_KEY_H;
                case KeyI: return GLFW_KEY_I;
                case KeyJ: return GLFW_KEY_J;
                case KeyK: return GLFW_KEY_K;
                case KeyL: return GLFW_KEY_L;
                case KeyM: return GLFW_KEY_M;
                case KeyN: return GLFW_KEY_N;
                case KeyO: return GLFW_KEY_O;
                case KeyP: return GLFW_KEY_P;
                case KeyQ: return GLFW_KEY_Q;
                case KeyS: return GLFW_KEY_S;
                case KeyT: return GLFW_KEY_T;
                case KeyU: return GLFW_KEY_U;
                case KeyV: return GLFW_KEY_V;
                case KeyW: return GLFW_KEY_W;
                case KeyX: return GLFW_KEY_X;
                case KeyY: return GLFW_KEY_Y;
                case KeyZ: return GLFW_KEY_Z;
                case KeyDelete: return GLFW_KEY_DELETE;
                case KeyNumpad0: return GLFW_KEY_KP_0;
                case KeyNumpad1: return GLFW_KEY_KP_1;
                case KeyNumpad2: return GLFW_KEY_KP_2;
                case KeyNumpad3: return GLFW_KEY_KP_3;
                case KeyNumpad4: return GLFW_KEY_KP_4;
                case KeyNumpad5: return GLFW_KEY_KP_5;
                case KeyNumpad6: return GLFW_KEY_KP_6;
                case KeyNumpad7: return GLFW_KEY_KP_7;
                case KeyNumpad8: return GLFW_KEY_KP_8;
                case KeyNumpad9: return GLFW_KEY_KP_9;
                case KeyNumpadPeriod: return GLFW_KEY_KP_DECIMAL;
                case KeyNumpadDivide: return GLFW_KEY_KP_DIVIDE;
                case KeyNumpadMultiply: return GLFW_KEY_KP_MULTIPLY;
                case KeyNumpadMinus: return GLFW_KEY_KP_SUBTRACT;
                case KeyNumpadPlus: return GLFW_KEY_KP_ADD;
                case KeyNumpadEnter: return GLFW_KEY_KP_ENTER;
                case KeyNumpadEquals: return GLFW_KEY_KP_EQUAL;
                case KeyUp: return GLFW_KEY_UP;
                case KeyDown: return GLFW_KEY_DOWN;
                case KeyLeft: return GLFW_KEY_LEFT;
                case KeyRight: return GLFW_KEY_RIGHT;
                case KeyInsert: return GLFW_KEY_INSERT;
                case KeyHome: return GLFW_KEY_HOME;
                case KeyEnd: return GLFW_KEY_END;
                case KeyPageUp: return GLFW_KEY_PAGE_UP;
                case KeyPageDown: return GLFW_KEY_PAGE_DOWN;
                case KeyF1: return GLFW_KEY_F1;
                case KeyF2: return GLFW_KEY_F2;
                case KeyF3: return GLFW_KEY_F3;
                case KeyF4: return GLFW_KEY_F4;
                case KeyF5: return GLFW_KEY_F5;
                case KeyF6: return GLFW_KEY_F6;
                case KeyF7: return GLFW_KEY_F7;
                case KeyF8: return GLFW_KEY_F8;
                case KeyF9: return GLFW_KEY_F9;
                case KeyF10: return GLFW_KEY_F10;
                case KeyF11: return GLFW_KEY_F11;
                case KeyF12: return GLFW_KEY_F12;
                case KeyNumLock: return GLFW_KEY_NUM_LOCK;
                case KeyCapsLock: return GLFW_KEY_CAPS_LOCK;
                case KeyScrollLock: return GLFW_KEY_SCROLL_LOCK;
                case KeyLeftShift: return GLFW_KEY_LEFT_SHIFT;
                case KeyRightShift: return GLFW_KEY_RIGHT_SHIFT;
                case KeyLeftCtrl: return GLFW_KEY_LEFT_CONTROL;
                case KeyRightCtrl: return GLFW_KEY_RIGHT_CONTROL;
                case KeyLeftAlt: return GLFW_KEY_LEFT_ALT;
                case KeyRightAlt: return GLFW_KEY_RIGHT_ALT;
                case KeyBreak: return GLFW_KEY_PAUSE;
                default: return 0;
            }
        }
    }



    Keyboard::Impl::Impl()
    {
    }

    Keyboard::Impl::~Impl()
    {
    }

    void Keyboard::Impl::handle(const Event& event)
    {
        switch(event.type)
        {
            case EventKeyboard:
                switch(event.keyboard.action)
                {
                    case GLFW_PRESS: keys[event.keyboard.key].pressed = true; break;
                    case GLFW_RELEASE: keys[event.keyboard.key].pressed = false; break;
                    default: break;
                }
                break;
            default: break;
        }
    }

    Keyboard::Keyboard()
        : impl(new Impl())
    {
    }

    Keyboard::~Keyboard()
    {
    }

    Input& Keyboard::operator[](Key k)
    {
        return impl->keys[getGLFWKey(k)];
    }



    Mouse::Impl::Impl()
        : x(0), y(0), scroll(0)
    {
    }

    Mouse::Impl::~Impl()
    {
    }

    void Mouse::Impl::handle(const Event& event)
    {
        switch(event.type)
        {
            case EventMouseClick:
                switch(event.mouse.click.action)
                {
                    case GLFW_PRESS: buttons[event.mouse.click.button].pressed = true; break;
                    case GLFW_RELEASE: buttons[event.mouse.click.button].pressed = false; break;
                    default: break;
                }
                break;
            case EventMouseMove:
                x = event.mouse.move.x;
                y = event.mouse.move.y;
                break;
            case EventMouseScroll:
                scroll += event.mouse.scroll.dy;
                break;
            default: break;
        }
    }

    Mouse::Mouse()
        : impl(new Impl())
    {
    }

    Mouse::~Mouse()
    {
    }

    double Mouse::getX() const
    {
        return impl->x;
    }

    double Mouse::getY() const
    {
        return impl->y;
    }

    double Mouse::getScroll() const
    {
        return impl->scroll;
    }

    Input& Mouse::left()
    {
        return impl->buttons[GLFW_MOUSE_BUTTON_LEFT];
    }

    Input& Mouse::middle()
    {
        return impl->buttons[GLFW_MOUSE_BUTTON_MIDDLE];
    }

    Input& Mouse::right()
    {
        return impl->buttons[GLFW_MOUSE_BUTTON_RIGHT];
    }



    namespace
    {
        const float Deadzone = 0.3f;
    }

    class Joystick::Impl
    {
        public:
            Impl(Engine& engine, unsigned int index)
                : engine(engine), index(index),
                active(false), axisCount(0), buttonCount(0)
            {
                hook = engine.addUpdateHook([this](){ update(); });
                std::memset(lastButtonState, 0, sizeof(lastButtonState));
                update();
            }

            ~Impl()
            {
            }

            void update()
            {
                active = glfwJoystickPresent(index) == GL_TRUE;
                if(!active)
                {
                    return;
                }

                {
                    int count;
                    const float* result = glfwGetJoystickAxes(index, &count);

                    axisCount = std::min((unsigned int) count, (unsigned int) AxisMax);

                    for(unsigned int i = 0; i < AxisMax; i++)
                    {
                        Axis& axis(axes[i]);
                        if(i >= axisCount)
                        {
                            axis.value = 0;
                        }
                        else
                        {
                            axis.value = result[i];
                        }

                        if(axis.value < -Deadzone)
                        {
                            if(axis.sign >= 0)
                            {
                                axis.plus.pressed = false;
                                axis.minus.pressed = true;
                            }
                            axis.sign = -1;
                        }
                        else if(axis.value > Deadzone)
                        {
                            if(axis.sign <= 0)
                            {
                                axis.plus.pressed = true;
                                axis.minus.pressed = false;
                            }
                            axis.sign = 1;
                        }
                        else
                        {
                            if(axis.sign != 0)
                            {
                                axis.plus.pressed = false;
                                axis.minus.pressed = false;
                            }
                            axis.sign = 0;
                        }
                    }
                }

                {
                    int count;
                    const unsigned char* result = glfwGetJoystickButtons(index, &count);

                    buttonCount = std::min((unsigned int) count, (unsigned int) ButtonMax);

                    for(unsigned int i = 0; i < ButtonMax; i++)
                    {
                        Input& button(buttons[i]);
                        if(i >= buttonCount)
                        {
                            button.pressed = false;
                        }
                        else
                        {
                            if(lastButtonState[i] != result[i])
                            {
                                switch(result[i])
                                {
                                    case GLFW_PRESS: button.pressed = true; break;
                                    case GLFW_RELEASE: button.pressed = false; break;
                                    default: break;
                                }
                            }

                            lastButtonState[i] = result[i];
                        }
                    }
                }
            }

            Engine& engine;
            unsigned int index;
            bool active;
            unsigned int axisCount, buttonCount;

            std::shared_ptr<Engine::UpdateHook> hook;

            unsigned char lastButtonState[ButtonMax];
            Input buttons[ButtonMax];
            Axis axes[AxisMax];
    };
        
    Joystick::Joystick(Engine& engine, unsigned int joystickIndex)
        : impl(new Impl(engine, joystickIndex))
    {
    }

    bool Joystick::isActive() const
    {
        return impl->active;
    }

    const char* Joystick::getName() const
    {
        return glfwGetJoystickName(impl->index);
    }

    unsigned int Joystick::getAxisCount() const
    {
        return impl->axisCount;
    }

    unsigned int Joystick::getButtonCount() const
    {
        return impl->buttonCount;
    }

    Axis& Joystick::axis(unsigned int axisIndex)
    {
        return impl->axes[axisIndex];
    }

    Input& Joystick::button(unsigned int buttonIndex)
    {
        return impl->buttons[buttonIndex];
    }
}
