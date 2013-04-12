#include <cmath>
#include <GL/glfw3.h>

#include "engine.h"
#include "../../core/input.h"
#include "../../core/screen.h"
#include "../../core/image.h"
#include "../../core/transform.h"

namespace plum
{
    class Screen::Impl
    {
        public:
            Impl(Engine& engine, bool windowed)
                : engine(engine),
                window(nullptr),
                interrupt(false),
                defaultClose(true),
                windowed(windowed),
                transformBound(false),
                trueWidth(0),
                trueHeight(0),
                previousWindowedX(0),
                previousWindowedY(0),
                previousWindowedWidth(0),
                previousWindowedHeight(0),
                width(0),
                height(0),
                scale(1),
                opacity(255)
            {
                hook = engine.addUpdateHook([this](){ update(); });
            }

            ~Impl()
            {
                if(window)
                {
                    glfwDestroyWindow(window);
                }
            }

            void update()
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
                        switch(e.type)
                        {
                            case EventResize:
                                resize(e.resize.width, e.resize.height, windowed);
                                break;
                            case EventClose:
                                if(defaultClose)
                                {
                                    engine.quit();
                                }
                                closeButton.setPressed(true);
                                break;
                        }
                        eventHooks.cleanup();
                    }
                    events.clear();
                }

                glfwSwapBuffers(window);
            }

            std::shared_ptr<Screen::EventHook> addEventHook(const EventHook& hook);
            void resize(int trueWidth, int trueHeight, bool windowed);

            WeakList<std::function<void(const Event&)>> eventHooks;
            std::vector<Event> events;

            Engine& engine;
            Input closeButton;
            Keyboard keyboard;

            std::shared_ptr<Engine::UpdateHook> hook;
            GLFWwindow* window;

            bool interrupt;
            bool defaultClose;
            bool windowed;
            bool transformBound;

            int trueWidth, trueHeight;
            int previousWindowedX, previousWindowedY;
            int previousWindowedWidth, previousWindowedHeight;
            int width, height;
            int scale;
            int opacity;
            std::string title;
    };

    namespace
    {
        void useHardwareBlender(BlendMode mode)
        {
            switch(mode)
            {
                case BlendOpaque:
                    glDisable(GL_BLEND);
                    break;
                case BlendMerge:
                case BlendPreserve:
                default:
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    break;
                case BlendAdd:
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                    break;
                case BlendSubtract:
                    glDisable(GL_BLEND);
                    break;
            }
        }

        void dispatch(GLFWwindow* window, const Event& event)
        {
            auto impl = (Screen::Impl*) glfwGetWindowUserPointer(window);
            if(!impl->interrupt)
            {
                impl->events.push_back(event);
            }
        }
    }

    Screen::Screen(Engine& engine, int width, int height, int scale, bool win)
        : impl(new Impl(engine, win))
    {
        setResolution(width, height, scale, win);
    }

    Screen::~Screen()
    {
    }

    bool Screen::getDefaultClose() const
    {
        return impl->defaultClose;
    }

    bool Screen::getWindowed() const
    {
        return impl->windowed;
    }

    int Screen::getWidth() const
    {
        return impl->width;
    }

    int Screen::getHeight() const
    {
        return impl->height;
    }

    int Screen::getTrueWidth() const
    {
        return impl->trueWidth;
    }

    int Screen::getTrueHeight() const
    {
        return impl->trueHeight;
    }

    int Screen::getOpacity() const
    {
        return impl->opacity;
    }

    const std::string& Screen::getTitle() const
    {
        return impl->title;
    }

    void Screen::setDefaultClose(bool value)
    {
        impl->defaultClose = value;
    }

    void Screen::setWindowed(bool value)
    {
        impl->resize(impl->width * impl->scale, impl->height * impl->scale, value);
    }

    void Screen::setOpacity(int value)
    {
        impl->opacity = value;
    }

    void Screen::setTitle(const std::string& value)
    {
        impl->title = value;
        glfwSetWindowTitle(impl->window, value.c_str());
    }

    Input& Screen::closeButton()
    {
        return impl->closeButton;
    }

    Keyboard& Screen::keyboard()
    {
        return impl->keyboard;
    }

    std::shared_ptr<Screen::EventHook> Screen::Impl::addEventHook(const EventHook& hook)
    {
        auto ptr = std::make_shared<Screen::EventHook>(hook);
        eventHooks.append(ptr);
        return ptr;
    }

    std::shared_ptr<Screen::EventHook> Screen::addEventHook(const EventHook& hook)
    {
        return impl->addEventHook(hook);
    }

    void Screen::Impl::resize(int trueWidth, int trueHeight, bool windowed)
    {
        interrupt = true;

        if(this->windowed != windowed)
        {
            // Unfortunately, since we can't apply new window hints after a window is
            // created, we must destroy and recreate this window.
            if(window)
            {
                if(this->windowed)
                {
                    if(!previousWindowedX)
                    {
                        GLFWvidmode mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
                        previousWindowedX = (mode.width - width * scale) / 2; 
                        previousWindowedY = (mode.height - height * scale) / 2; 
                    }
                    else
                    {
                        glfwGetWindowPos(window, &previousWindowedX, &previousWindowedY);
                    }
                    glfwGetWindowSize(window, &previousWindowedWidth, &previousWindowedHeight);
                }

                glfwDestroyWindow(window);
                window = nullptr;
            }
            this->windowed = windowed;
        }

        if(!previousWindowedWidth)
        {
            int x, y;

            GLFWvidmode mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            glfwGetMonitorPos(glfwGetPrimaryMonitor(), &x, &y);

            previousWindowedX =  (mode.width - width * scale) / 2 + x; 
            previousWindowedY = (mode.height - height * scale) / 2 + y;
            previousWindowedWidth = width * scale;
            previousWindowedHeight = height * scale;
        }

        // TODO: borderless fake fullscreen mode.
        if(!window)
        {
            glfwDefaultWindowHints();
            glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

            int dx = 0;
            int dy = 0;
            int w = trueWidth;
            int h = trueHeight;

            if(!windowed)
            {
                int count;
                GLFWmonitor** monitors = glfwGetMonitors(&count);
                GLFWmonitor* dest = monitors[0];
                for(int i = 1; i < count; i++)
                {
                    int x, y;
                    glfwGetMonitorPos(monitors[i], &x, &y);
                    GLFWvidmode mode = glfwGetVideoMode(dest);

                    if(previousWindowedX >= x
                        && previousWindowedX < x + mode.width 
                        && previousWindowedY >= y
                        && previousWindowedY < y + mode.height)
                    {
                        dest = monitors[i];
                        dx = x;
                        dy = y;
                    }
                }

                GLFWvidmode mode = glfwGetVideoMode(dest);
                w = mode.width;
                h = mode.height;

                glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
                glfwWindowHint(GLFW_DECORATED, GL_FALSE);
            }
            if(windowed)
            {
                dx = previousWindowedX;
                dy = previousWindowedY;
                w = previousWindowedWidth;
                h = previousWindowedHeight;
            }

            auto window = glfwCreateWindow(w, h, title.c_str(), nullptr, engine.impl->root);
            if(!window)
            {
                throw std::runtime_error("Screen settings were not compatible your graphics card.\r\n");
            }
           
            glfwSetWindowPos(window, dx, dy);

            glfwSetWindowUserPointer(window, this);
            glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
            {
                glfwSetWindowShouldClose(window, GL_FALSE);

                Event event;
                event.type = EventClose;
                event.window = window;
                dispatch(window, event);
            });

            glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int action)
            {
                Event event;
                event.type = EventKeyboard;
                event.window = window;
                event.keyboard.key = key;
                event.keyboard.action = action;
                dispatch(window, event);
            });

            glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int w, int h)
            {
                Event event;
                event.type = EventResize;
                event.window = window;
                event.resize.width = w;
                event.resize.height = h;
                dispatch(window, event);
            });

            keyboard.impl->hook = addEventHook([this](const Event& event){ keyboard.impl->handle(event); });

            glfwSwapInterval(1);
            glfwShowWindow(window);

            this->window = window;
        }
        if(windowed)
        {
            glfwGetWindowSize(window, &trueWidth, &trueHeight);
            scale = std::max(std::min((trueWidth + width / 2) / width, (trueWidth + height / 2) / height), 1);
            glfwSetWindowSize(window, width * scale, height * scale);
        }

        glfwGetWindowSize(window, &trueWidth, &trueHeight);
        scale = std::max(std::min(trueWidth / width, trueHeight / height), 1);

        glfwMakeContextCurrent(window);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, width, height, 0, -1, 1);

        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glViewport((trueWidth - width * scale) / 2, (trueHeight - height * scale) / 2, width * scale, height * scale);
        glLineWidth(float(scale));

        glDisable(GL_DEPTH_TEST);

        glScissor((trueWidth - width * scale) / 2, (trueHeight - height * scale) / 2, width * scale, height * scale);
        glEnable(GL_SCISSOR_TEST);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        this->trueWidth = trueWidth;
        this->trueHeight = trueHeight;
        interrupt = false;
    }

    void Screen::setResolution(int width, int height, int scale, bool win)
    {
        impl->width = width;
        impl->height = height;
        impl->scale = scale;

        impl->resize(width * scale, height * scale, win);
    }

    void Screen::bind(Image& image)
    {
        glfwMakeContextCurrent(impl->window);
        glColor4ub(255, 255, 255, getOpacity());
        useHardwareBlender(BlendPreserve);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glEnable(GL_TEXTURE_2D);
        image.bindRaw();
    }

    void Screen::bind(const Transform& transform, int x, int y, int width, int height)
    {
        glfwMakeContextCurrent(impl->window);

        uint8_t r, g, b, a;
        transform.tint.channels(r, g, b, a);
        glColor4ub(r, g, b, a * getOpacity() / 255);
        useHardwareBlender(transform.mode);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glPushMatrix();
        glTranslated(x, y, 0.0);
        glTranslated(width / 2, height / 2, 0.0);
        glScaled(transform.scaleX * (1 - transform.mirror * 2), transform.scaleY, 0.0);
        glRotated(transform.angle, 0.0, 0.0, 1.0);
        glTranslated(-width / 2, -height / 2, 0.0);
        impl->transformBound = true;
    }

    void Screen::bind(Image& image, const Transform& transform, int x, int y, int width, int height)
    {
        bind(transform, x, y, width, height);

        glEnable(GL_TEXTURE_2D);
        image.bindRaw();
    }

    void Screen::unbind()
    {
        if(impl->transformBound)
        {
            glPopMatrix();
        }
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    void Screen::clear(Color color)
    {
        uint8_t r, g, b, a;
        color.channels(r, g, b, a);
        if(a * getOpacity() / 255 == 255)
        {
            glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }
        else
        {
            clear(0, 0, impl->width, impl->height, color);
        }
    }

    void Screen::clear(int x, int y, int x2, int y2, Color color)
    {
        glfwMakeContextCurrent(impl->window);

        uint8_t r, g, b, a;
        color.channels(r, g, b, a);
        glColor4ub(r, g, b, a * getOpacity() / 255);
        useHardwareBlender(BlendPreserve);
        glDisable(GL_TEXTURE_2D);

        if(x > x2)
        {
            std::swap(x, x2);
        }
        if(y > y2)
        {
            std::swap(y, y2);
        }

        const GLdouble vertexArray[] = {
            x - 0.5, y - 0.5,
            x2 + 0.5, y - 0.5,
            x2 + 0.5, y2 + 0.5,
            x - 0.5, y2 + 0.5,
        };

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_DOUBLE, 0, vertexArray);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        glfwMakeContextCurrent(impl->engine.impl->root);
    }
}
