#pragma once

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

            void handleMouseButtonEvent(SDL_MouseButtonEvent e);
            void poll();
            void refresh(Script& script);

            void setTitle(std::string title);
            void setResolution(int width, int height, bool windowed);
    };
}
