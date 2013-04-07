#ifndef PLUM_GLFW_ENGINE_H
#define PLUM_GLFW_ENGINE_H

#include <GL/glfw3.h>
#include <vector>

#include "../../core/engine.h"

namespace plum
{
    enum EventType
    {
        EventClose,
        EventKeyboard,
    };

    class Event
    {
        public:
            EventType type;
            union
            {
                struct
                {
                    GLFWwindow window;
                } close;
                struct
                {
                    GLFWwindow window;
                    int key, action;
                } keyboard;
            };
    };

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

    class WindowContext
    {
        public:
            WindowContext(Engine::Impl* impl, GLFWwindow window)
                : impl_(impl), window_(window)
            {
            }

            ~WindowContext()
            {
                glfwDestroyWindow(window_);
            }

            GLFWwindow window() const
            {
                return window_;
            }

            Engine::Impl* impl() const
            {
                return impl_;
            }

        private:
            Engine::Impl* impl_;
            GLFWwindow window_;
    };

    class Engine::Impl
    {
        public:
            WeakList<std::function<void(const Event&)>> eventHooks;
            WeakList<std::function<void()>> updateHooks;
            WeakList<WindowContext> windows;
            std::vector<Event> events;

            Impl()
            {
                if(!glfwInit())
                {
                    quit("Couldn't initialize glfw.\n");
                }
                glfwSetTime(0.0);
            }

            ~Impl()
            {
                glfwTerminate();
            }

            void quit(const std::string& message);
            std::shared_ptr<WindowContext> registerWindow(GLFWwindow win);
            void refresh();
    };
}

#endif