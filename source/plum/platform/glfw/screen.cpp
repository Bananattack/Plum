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
            Impl(Engine& engine)
                : engine(engine),
                windowed(true),
                transformBound(false),
                trueWidth(0),
                trueHeight(0),
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
                        if(e.type == EventClose)
                        {
                            closeButton.setPressed(true);
                        }
                        eventHooks.cleanup();
                    }
                    events.clear();
                }

                glfwSwapBuffers(window);
            }

            WeakList<std::function<void(const Event&)>> eventHooks;
            std::vector<Event> events;

            Engine& engine;
            Input closeButton;
            Keyboard keyboard;

            std::shared_ptr<Engine::UpdateHook> hook;
            GLFWwindow window;

            bool windowed;
            bool transformBound;

            int trueWidth, trueHeight;
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

        void dispatch(GLFWwindow window, const Event& event)
        {
            auto screen = (Screen*) glfwGetWindowUserPointer(window);
            screen->impl->events.push_back(event);
        }
    }

    Screen::Screen(Engine& engine, int width, int height, int scale, bool win)
        : impl(new Impl(engine))
    {
        setResolution(width, height, scale, win);
    }

    Screen::~Screen()
    {
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

    void Screen::setOpacity(int value)
    {
        impl->opacity = value;
    }

    void Screen::setTitle(const std::string& title)
    {
        impl->title = title;
        glfwSetWindowTitle(impl->window, title.c_str());
    }

    Input& Screen::closeButton()
    {
        return impl->closeButton;
    }

    Keyboard& Screen::keyboard()
    {
        return impl->keyboard;
    }

    std::shared_ptr<Screen::EventHook> Screen::addEventHook(const EventHook& hook)
    {
        auto ptr = std::make_shared<Screen::EventHook>(hook);
        impl->eventHooks.append(ptr);
        return ptr;
    }

    void Screen::setResolution(int width, int height, int scale, bool win)
    {
        impl->windowed = win;

        impl->width = width;
        impl->height = height;
        impl->scale = scale;

        // Fullscreen is usually not supported for low-res, so upscale everything!
        if(!impl->windowed && (width < 640 || height < 480))
        {
            width = 640;
            height = 480;
        }

        auto window = glfwCreateWindow(width * scale, height * scale, (impl->windowed ? GLFW_WINDOWED : GLFW_FULLSCREEN), "", impl->engine.impl->root);
        if(!window)
        {
            throw std::runtime_error("Screen settings were not compatible your graphics card.\r\n");
        }
        glfwGetWindowSize(window, &impl->trueWidth, &impl->trueHeight);

        glfwMakeContextCurrent(window);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, impl->width, impl->height, 0, -1, 1);
        glViewport(0, 0, impl->trueWidth, impl->trueHeight);
        glLineWidth(float(scale));

        glDisable(GL_DEPTH_TEST);

        glScissor(0, 0, impl->trueWidth, impl->trueHeight);
        glEnable(GL_SCISSOR_TEST);

        glClearColor(0.0, 0.0, 0.0, 1.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glfwSwapInterval(1);
        glfwShowWindow(window);

        glfwSetWindowUserPointer(window, this);
        glfwSetWindowCloseCallback(window, [](GLFWwindow window)
        {
            Event event;
            event.type = EventClose;
            event.window = window;
            dispatch(window, event);
            return GL_TRUE;
        });

        glfwSetKeyCallback(window, [](GLFWwindow window, int key, int action)
        {
            Event event;
            event.type = EventKeyboard;
            event.window = window;
            event.keyboard.key = key;
            event.keyboard.action = action;
            dispatch(window, event);
        });

        impl->keyboard.impl->hook = addEventHook([this](const Event& event){ impl->keyboard.impl->handle(event); });

        impl->window = window;
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
