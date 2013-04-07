#include <iostream>

#include "engine.h"
#include "../../core/log.h"

#ifdef _WIN32
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
            impl->events.push_back(event);
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
        glfwSetWindowCloseCallback(window, [](GLFWwindow window)
        {
            Event event;
            event.type = EventClose;
            event.close.window = window;
            dispatch(window, event);
            return GL_TRUE;
        });

        glfwSetMouseButtonCallback(window, [](GLFWwindow window, int button, int action)
        {
            Event event;
            event.type = EventMouseButton;
            event.mouse.button.window = window;
            event.mouse.button.button = button;
            event.mouse.button.action = action;
            dispatch(window, event);
        });

        glfwSetCursorPosCallback(window, [](GLFWwindow window, int x, int y)
        {
            Event event;
            event.type = EventMouseMove;
            event.keyboard.window = window;
            event.mouse.move.x = x;
            event.mouse.move.y = y;
            dispatch(window, event);
        });

        glfwSetScrollCallback(window, [](GLFWwindow window, double dx, double dy)
        {
            Event event;
            event.type = EventMouseScroll;
            event.keyboard.window = window;
            event.mouse.scroll.dx = dx;
            event.mouse.scroll.dy = dy;
            dispatch(window, event);
        });

        glfwSetKeyCallback(window, [](GLFWwindow window, int key, int action)
        {
            Event event;
            event.type = EventKeyboard;
            event.keyboard.window = window;
            event.keyboard.key = key;
            event.keyboard.action = action;
            dispatch(window, event);
        });
        
        return ptr;
    }

    void Engine::Impl::refresh()
    {
        while(true)
        {
            glfwPollEvents();
            if(events.size() == 0)
            {
                break;
            }
            for(const auto& e : events)
            {
				for(const auto& h : eventHooks)
                {
                    if(auto f = h.lock())
                    {
                        (*f)(e);
                    }
                }
                if(e.type == EventClose)
                {
                    quit("");
                }
                eventHooks.cleanup();
            }
            events.clear();
        }

        for(const auto h : updateHooks)
        {
            if(auto f = h.lock())
            {
                (*f)();
            }
        }
        updateHooks.cleanup();
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
        impl->refresh();
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
