#pragma once

#include <vector>
#include <stdexcept>
#include <functional>

#include "../common/config.h"
#include "../video/video.h"
#include "../audio/audio_audiere.h"
#include "timer.h"
#include "mouse.h"
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

            int status()
            {
                return status_;
            }

        private:
            int status_;
    };

    class Engine
    {
        private:
            std::string titlePrefix;

            bool initialized;
            bool destroyed;

            std::vector<std::function<void()>> updateHooks;

        public:
            Config config;
            Video video;
            Audio audio;
            Timer timer;
            Mouse mouse;
            KeyInput key[KEY_MAX_COUNT];

        public:
            Engine();
            ~Engine();

            void quit(std::string message = "");

            size_t addUpdateHook(const std::function<void()>& hook);
            void removeUpdateHook(size_t index);

            void handleMouseButtonEvent(SDL_MouseButtonEvent e);
            void poll();
            void refresh();

            void setTitle(std::string title);
            void setResolution(int width, int height, bool windowed);
    };
}
