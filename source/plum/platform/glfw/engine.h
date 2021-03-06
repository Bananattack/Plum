#ifndef PLUM_GLFW_ENGINE_H
#define PLUM_GLFW_ENGINE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "../../core/engine.h"
#include "../../core/screen.h"
#include "../../core/input.h"

namespace plum
{
    enum EventType
    {
        EventClose,
        EventKeyboard,
        EventResize,
        EventMouseClick,
        EventMouseMove,
        EventMouseScroll
    };

    class Event
    {
        public:
            EventType type;
            GLFWwindow* window;
            union
            {
                struct
                {
                    int width, height;
                } resize;
                struct
                {
                    int key, action;
                } keyboard;
                union
                {
                    struct
                    {
                        int button, action;
                    } click;
                    struct
                    {
                        double x, y;
                    } move;
                    struct
                    {
                        double dx, dy;
                    } scroll;
                } mouse;
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

    class Keyboard::Impl
    {
        public:
            Impl();
            ~Impl();

            void handle(const Event& event);
            
            std::shared_ptr<Screen::EventHook> hook;
            Input keys[GLFW_KEY_LAST];
    };

    class Mouse::Impl
    {
        public:
            Impl();
            ~Impl();

            void handle(const Event& event);
            
            std::shared_ptr<Screen::EventHook> hook;
            Input buttons[GLFW_MOUSE_BUTTON_LAST];
            double x, y, scroll;
    };

    class Engine::Impl
    {
        public:
            WeakList<std::function<void()>> updateHooks;
			GLFWwindow* root;

            bool coreProfile, modernPipeline;
            int majorVersion, minorVersion;
            GLuint program;
            GLuint fragmentShader;
            GLuint vertexShader;
            GLint projectionUniform, originUniform, pivotUniform, scaleUniform, angleUniform, colorUniform;
            GLint xyAttribute, uvAttribute;

            bool windowless;

            Impl(Config& config);
            ~Impl();

            void quit(const std::string& message);
            void refresh();
    };
}

#endif