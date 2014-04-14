#ifndef PLUM_ENGINE_H
#define PLUM_ENGINE_H

#include <vector>
#include <stdexcept>
#include <functional>
#include <memory>
#include <string>

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

    class Event;
    class Config;

    class Engine
    {
        public:
            Engine(Config& config);
            ~Engine();

            void refresh();
            void quit(const std::string& message = "");

            typedef std::function<void()> UpdateHook;
            std::shared_ptr<UpdateHook> addUpdateHook(const UpdateHook& hook);

            class Impl;
            std::shared_ptr<Impl> impl;
    };
}

#endif
