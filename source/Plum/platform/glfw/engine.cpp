#include <iostream>

#include "engine.h"
#include "../../core/log.h"

#ifdef _WIN32
#define NOMINMAX
#include <GL/glfw3native.h>
#endif

namespace plum
{
    namespace
    {
        void dispatch(GLFWwindow window, const Event& event)
        {
            auto context = (WindowContext*) glfwGetWindowUserPointer(window);
            auto impl = context->impl();
            if(impl->eventCount < impl->events.size())
            {
                impl->events[impl->eventCount++] = event;
            }
            else
            {
                impl->events.push_back(event);
            }
        }

        int handleClose(GLFWwindow window)
        {
            Event event;
            event.type = EventClose;
            event.close.window = window;
            dispatch(window, event);
            return GL_TRUE;
        }

        void handleMouseButton(GLFWwindow window, int button, int action)
        {
            Event event;
            event.type = EventMouseButton;
            event.mouse.button.window = window;
            event.mouse.button.button = button;
            event.mouse.button.action = action;
            dispatch(window, event);
        }

        void handleMouseMove(GLFWwindow window, int x, int y)
        {
            Event event;
            event.type = EventMouseMove;
            event.keyboard.window = window;
            event.mouse.move.x = x;
            event.mouse.move.y = y;
            dispatch(window, event);
        }

        void handleMouseScroll(GLFWwindow window, double dx, double dy)
        {
            Event event;
            event.type = EventMouseScroll;
            event.keyboard.window = window;
            event.mouse.scroll.dx = dx;
            event.mouse.scroll.dy = dy;
            dispatch(window, event);
        }

        void handleKeyboard(GLFWwindow window, int key, int action)
        {
            Event event;
            event.type = EventKeyboard;
            event.keyboard.window = window;
            event.keyboard.key = key;
            event.keyboard.action = action;
            dispatch(window, event);
        }
    }

    void Engine::Impl::quit(const std::string& message)
    {
        if(message.length())
        {
            fprintf(stderr, "Exit Requested: %s", message.c_str());
            logFormat("Exit Requested: %s", message.c_str());
#ifdef _WIN32
            MessageBoxA(nullptr, message.c_str(), "Exit Requested", MB_SYSTEMMODAL);
#endif
            throw SystemExit(1);
        }
        throw SystemExit(0);
    }

    std::shared_ptr<WindowContext> Engine::Impl::registerWindow(GLFWwindow window)
    {
        auto ptr = std::make_shared<WindowContext>(this, window);
        windows.append(ptr);

        glfwSetWindowUserPointer(window, ptr.get());
        glfwSetWindowCloseCallback(window, handleClose);
        glfwSetMouseButtonCallback(window, handleMouseButton);
        glfwSetCursorPosCallback(window, handleMouseMove);
        glfwSetScrollCallback(window, handleMouseScroll);
        glfwSetKeyCallback(window, handleKeyboard);
        
        return ptr;
    }

    Engine::Engine()
        : impl(new Impl())
    {
    }

    Engine::~Engine()
    {
    }

    void Engine::refresh()
    {
        while(true)
        {
            glfwPollEvents();
            if(impl->eventCount == 0)
            {
                break;
            }

            auto& events(impl->events);
            for(size_t i = 0, count = impl->eventCount; i != count; ++i)
            {
                const auto& e(events[i]);
                auto& eventHooks(impl->eventHooks);
                for(auto h = eventHooks.begin(), hookEnd = eventHooks.end(); h != hookEnd; ++h)
                {
                    if(auto f = h->lock())
                    {
                        (*f)(e);
                    }
                }
                if(e.type == EventClose)
                {
                    quit();
                }
                eventHooks.cleanup();
            }
            impl->eventCount = 0;
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
