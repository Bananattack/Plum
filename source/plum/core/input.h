#ifndef PLUM_INPUT_H
#define PLUM_INPUT_H

#include <memory>

namespace plum
{
    class Engine;
    class Input
    {
        public:
            Input();
            ~Input();

            bool isPressed();
            void setPressed(bool value);

        private:
            bool pressed;
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
            Keyboard(Engine& engine);
            ~Keyboard();

            Input& operator[](Key k);

            class Impl;
            std::shared_ptr<Impl> impl;
    };
}

#endif