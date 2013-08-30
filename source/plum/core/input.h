#ifndef PLUM_INPUT_H
#define PLUM_INPUT_H

#include <memory>
#include <string>

namespace plum
{
    class Engine;
    class Screen;
    struct Input
    {
        Input();
        ~Input();

        bool pressed;
    };

    struct Axis
    {
        Axis();
        ~Axis();

        double value;
        int sign;
        Input plus, minus;
    };

    enum Key
    {
        KeyBackspace,
        KeyTab,
        KeyEnter,
        KeyPause,
        KeyEscape,
        KeySpace,
        KeyExclaim,
        KeyQuote,
        KeyHash,
        KeyDollar,
        KeyAmpersand,
        KeyApostrophe,
        KeyLeftParen,
        KeyRightParen,
        KeyAsterisk,
        KeyPlus,
        KeyComma,
        KeyMinus,
        KeyPeriod,
        KeySlash,
        Key0,
        Key1,
        Key2,
        Key3,
        Key4,
        Key5,
        Key6,
        Key7,
        Key8,
        Key9,
        KeyColon,
        KeySemiColon,
        KeyLessThan,
        KeyEquals,
        KeyGreaterThan,
        KeyQuestion,
        KeyAt,
        KeyLeftBracket,
        KeyBackslash,
        KeyRightBracket,
        KeyCaret,
        KeyUnderscore,
        KeyTilde,
        KeyBackquote,
        KeyGrave,
        KeyA,
        KeyB,
        KeyC,
        KeyD,
        KeyE,
        KeyF,
        KeyG,
        KeyH,
        KeyI,
        KeyJ,
        KeyK,
        KeyL,
        KeyM,
        KeyN,
        KeyO,
        KeyP,
        KeyQ,
        KeyR,
        KeyS,
        KeyT,
        KeyU,
        KeyV,
        KeyW,
        KeyX,
        KeyY,
        KeyZ,
        KeyDelete,
        KeyNumpad0,
        KeyNumpad1,
        KeyNumpad2,
        KeyNumpad3,
        KeyNumpad4,
        KeyNumpad5,
        KeyNumpad6,
        KeyNumpad7,
        KeyNumpad8,
        KeyNumpad9,
        KeyNumpadPeriod,
        KeyNumpadDivide,
        KeyNumpadMultiply,
        KeyNumpadMinus,
        KeyNumpadPlus,
        KeyNumpadEnter,
        KeyNumpadEquals,
        KeyUp,
        KeyDown,
        KeyLeft,
        KeyRight,
        KeyInsert,
        KeyHome,
        KeyEnd,
        KeyPageUp,
        KeyPageDown,
        KeyF1,
        KeyF2,
        KeyF3,
        KeyF4,
        KeyF5,
        KeyF6,
        KeyF7,
        KeyF8,
        KeyF9,
        KeyF10,
        KeyF11,
        KeyF12,
        KeyNumLock,
        KeyCapsLock,
        KeyScrollLock,
        KeyLeftShift,
        KeyRightShift,
        KeyLeftCtrl,
        KeyRightCtrl,
        KeyLeftAlt,
        KeyRightAlt,
        KeyBreak,
    };
    
    class Keyboard
    {
        public:
            Keyboard();
            ~Keyboard();

            Input& operator[](Key k);

            class Impl;
            std::shared_ptr<Impl> impl;
    };

    class Mouse
    {
        public:
            Mouse();
            ~Mouse();

            double getX() const;
            double getY() const;
            double getScroll() const;

            Input& left();
            Input& middle();
            Input& right();

            class Impl;
            std::shared_ptr<Impl> impl;
    };

    class Joystick
    {
        public:
            static const unsigned int ButtonMax = 32;
            static const unsigned int AxisMax = 8;

            Joystick(Engine& engine, unsigned int joystickIndex);
            ~Joystick();

            bool isActive() const;
            const char* getName() const;
            unsigned int getAxisCount() const;
            unsigned int getButtonCount() const;

            Axis& axis(unsigned int axisIndex);
            Input& button(unsigned int buttonIndex);

            class Impl;
            std::shared_ptr<Impl> impl;
    };
}

#endif
