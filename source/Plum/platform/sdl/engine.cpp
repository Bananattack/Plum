#include <iostream>

#include "event.h"
#include "../../core/log.h"
#include "../../core/engine.h"

#ifdef _WIN32
#define NOMINMAX
#include <SDL_syswm.h>
#endif

#include <SDL.h>

namespace plum
{
    template<typename T> class WeakList
    {
        public:
            typedef typename std::vector<std::weak_ptr<T>>::iterator Iterator;
            typedef typename std::vector<std::weak_ptr<T>>::const_iterator ConstIterator;

            void append(const std::shared_ptr<T>& item)
            {
                items.push_back(item);
            }

            void cleanup()
            {
                for(auto it = begin(); it != end();)
                {
                    if(!it->lock())
                    {
                        it = items.erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                }
            }

            void clear() { items.clear(); }
            Iterator begin() { return items.begin(); }
            Iterator end() { return items.end(); }
            ConstIterator begin() const { return items.begin(); }
            ConstIterator end() const { return items.end(); }

        private:
            std::vector<std::weak_ptr<T>> items;
    };

    class Engine::Impl
    {
        public:
            WeakList<std::function<void(const Event&)>> eventHooks;
            WeakList<std::function<void()>> updateHooks;

            Impl()
            {
                if(SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE) != 0)
                {
                    quit("Couldn't initialize SDL.\n");
                }

        #ifdef _WIN32
                {
                    SDL_SysWMinfo info;
                    SDL_VERSION(&info.version);
                    auto windowHandle = SDL_GetWMInfo(&info) ? info.window : nullptr;
                    if(windowHandle)
                    {
                        SetClassLong(windowHandle, GCL_HICON, long(LoadIcon(GetModuleHandle(0), "APPICON")));
                    }
                }
        #endif
            }

            ~Impl()
            {
                SDL_Quit();
            }

            void quit(std::string message) 
            {
                if(message.length())
                {
                    fprintf(stderr, "exit message: %s", message.c_str());
        #ifdef _WIN32
                    SDL_SysWMinfo info;
                    SDL_VERSION(&info.version);
                    HWND hWnd = SDL_GetWMInfo(&info) ? info.window : HWND_DESKTOP;

                    MessageBoxA(hWnd, message.c_str(), "Error", 0);
        #endif
                    throw SystemExit(1);
                }
                throw SystemExit(0);
            }
    };


    Engine::Engine()
        : impl(new Impl())
    {
    }

    Engine::~Engine()
    {
    }


    void Engine::refresh()
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            Event event(e);
            auto& eventHooks(impl->eventHooks);
            for(auto it = eventHooks.begin(), end = eventHooks.end(); it != end; ++it)
            {
                if(auto f = it->lock())
                {
                    (*f)(event);
                }
            }
            if(e.type == SDL_QUIT)
            {
                quit();
            }
            eventHooks.cleanup();
        }

        auto& updateHooks(impl->updateHooks);
        for(auto it = updateHooks.begin(), end = updateHooks.end(); it != end; ++it)
        {
            if(auto f = it->lock())
            {
                (*f)();
            }
        }
        updateHooks.cleanup();
    }

    void Engine::quit(const std::string& message)
    {
        impl->quit(message);
    }

    std::shared_ptr<Engine::EventHook> Engine::addEventHook(const EventHook& hook)
    {
        auto ptr = std::make_shared<Engine::EventHook>(hook);
        impl->eventHooks.append(ptr);
        return ptr;
    }

    std::shared_ptr<Engine::UpdateHook> Engine::addUpdateHook(const UpdateHook& hook)
    {
        auto ptr = std::make_shared<Engine::UpdateHook>(hook);
        impl->updateHooks.append(ptr);
        return ptr;
    }
}
