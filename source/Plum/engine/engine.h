#pragma once

#include <vector>
#include <stdexcept>
#include <functional>

#include "mouse.h"
#include "timer.h"
#include "config.h"
#include "keyboard.h"

namespace plum
{
    class SystemExit : public std::exception
    {
        public:
            SystemExit(int status)
                : status_(status)
            {
            }

            const char* what() const throw()
            {
                return "SystemExit";
            }

            int status() const
            {
                return status_;
            }

        private:
            int status_;
    };

    class Engine
    {
        private:
            std::vector<std::function<void()>> updateHooks;

        public:
            Timer timer;
            Mouse mouse;
            KeyInput key[KEY_MAX_COUNT];

            Engine();
            ~Engine();

            void quit(std::string message = "");

            size_t addUpdateHook(const std::function<void()>& hook);
            void removeUpdateHook(size_t index);

            void handleMouseButtonEvent(SDL_MouseButtonEvent e);
            void poll();
            void refresh();

        private:
            Engine(const Engine&);
            void operator =(const Engine&);
    };
}
