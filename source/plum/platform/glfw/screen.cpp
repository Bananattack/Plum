#include <cmath>

#include "engine.h"
#include "../../core/input.h"
#include "../../core/screen.h"
#include "../../core/image.h"
#include "../../core/canvas.h"
#include "../../core/transform.h"

namespace plum
{
    namespace
    {
        const size_t VertexBufferSize = 6 * 4;
    }

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
                width(0), height(0),
                left(0), bottom(0),
                clipX(0), clipY(0), clipX2(0), clipY2(0),
                scale(1),
                opacity(255),
                vbo(0),
                defaultTexture(0),
                mode(BlendMode::Preserve)
            {
                hook = engine.addUpdateHook([this](){ update(); });
            }

            ~Impl()
            {
                if(window)
                {
                    glfwDestroyWindow(window);
                }
                if(vbo)
                {
                    glDeleteBuffers(1, &vbo);
                }
                if(defaultTexture)
                {
                    glDeleteTextures(1, &defaultTexture);
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
                                closeButton.pressed = true;
                                break;
                        }
                        eventHooks.cleanup();
                    }
                    events.clear();
                }

                glfwSwapBuffers(window);

                glfwMakeContextCurrent(window);
                useHardwareBlender(BlendMode::Opaque);

                engine.impl->windowless = false;
            }

            std::shared_ptr<Screen::EventHook> addEventHook(const EventHook& hook);
            void resize(int trueWidth, int trueHeight, bool windowed);

            WeakList<std::function<void(const Event&)>> eventHooks;
            std::vector<Event> events;

            Engine& engine;
            Input closeButton;
            Keyboard keyboard;
            Mouse mouse;

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
            int left, bottom;
            int clipX, clipY, clipX2, clipY2;
            int scale;
            int opacity;
            std::string title;

            GLuint vbo;
            GLuint defaultTexture;

            BlendMode mode;

            void useHardwareBlender(BlendMode mode)
            {
                if(this->mode != mode)
                {
                    switch(mode)
                    {
                        case BlendMode::Opaque:
                            glDisable(GL_BLEND);
                            break;
                        case BlendMode::Merge:
                        case BlendMode::Preserve:
                        default:
                            glEnable(GL_BLEND);
                            glBlendEquation(GL_FUNC_ADD);
                            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                            break;
                        case BlendMode::Add:
                            glEnable(GL_BLEND);
                            glBlendEquation(GL_FUNC_ADD);
                            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                            break;
                        case BlendMode::Subtract:
                            glEnable(GL_BLEND);
                            glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
                            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                            break;
                    }
                    this->mode = mode;
                }
            }
    };

    namespace
    {
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

    void Screen::getClipRegion(int& x, int& y, int& x2, int& y2) const
    {
        x = impl->clipX;
        y = impl->clipY;
        x2 = impl->clipX2;
        y2 = impl->clipY2;
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

    void Screen::restoreClipRegion()
    {
        setClipRegion(0, 0, impl->width - 1, impl->height - 1);
    }

    void Screen::setClipRegion(int x, int y, int x2, int y2)
    {
        if(x > x2)
        {
            std::swap(x, x2);
        }
        if(y > y2)
        {
            std::swap(y, y2);
        }
        impl->clipX = std::min(std::max(0, x), impl->width - 1);
        impl->clipY = std::min(std::max(0, y), impl->height - 1);
        impl->clipX2 = std::min(std::max(0, x2), impl->width - 1);
        impl->clipY2 = std::min(std::max(0, y2), impl->height - 1);
        glScissor(impl->left + impl->clipX * impl->scale, impl->bottom + (impl->height - 1 - impl->clipY2) * impl->scale, (impl->clipX2 - impl->clipX + 1) * impl->scale, (impl->clipY2 - impl->clipY + 1) * impl->scale);
    }

    Input& Screen::closeButton()
    {
        return impl->closeButton;
    }

    Keyboard& Screen::keyboard()
    {
        return impl->keyboard;
    }

    Mouse& Screen::mouse()
    {
        return impl->mouse;
    }

    Engine& Screen::engine()
    {
        return impl->engine;
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
                        auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
                        previousWindowedX = (mode->width - width * scale) / 2; 
                        previousWindowedY = (mode->height - height * scale) / 2; 
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

            auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            glfwGetMonitorPos(glfwGetPrimaryMonitor(), &x, &y);

            previousWindowedX =  (mode->width - width * scale) / 2 + x; 
            previousWindowedY = (mode->height - height * scale) / 2 + y;
            previousWindowedWidth = width * scale;
            previousWindowedHeight = height * scale;
        }

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
                    auto mode = glfwGetVideoMode(dest);

                    if(previousWindowedX >= x
                        && previousWindowedX < x + mode->width 
                        && previousWindowedY >= y
                        && previousWindowedY < y + mode->height)
                    {
                        dest = monitors[i];
                        dx = x;
                        dy = y;
                    }
                }

                auto mode = glfwGetVideoMode(dest);
                w = mode->width;
                h = mode->height;

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
            glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int modifiers)
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
            glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int modifiers)
            {
                Event event;
                event.type = EventMouseClick;
                event.window = window;
                event.mouse.click.button = button;
                event.mouse.click.action = action;
                dispatch(window, event);
            });
            glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y)
            {
                Event event;
                auto impl = (Screen::Impl*) glfwGetWindowUserPointer(window);
                event.type = EventMouseMove;
                event.window = window;
                event.mouse.move.x = (x - (impl->trueWidth - impl->width * impl->scale) / 2) / impl->scale;
                event.mouse.move.y = (y - (impl->trueHeight - impl->height * impl->scale) / 2) / impl->scale;
                dispatch(window, event);
            });
            glfwSetScrollCallback(window, [](GLFWwindow* window, double dx, double dy)
            {
                Event event;
                event.type = EventMouseScroll;
                event.window = window;
                event.mouse.scroll.dx = dx;
                event.mouse.scroll.dy = dy;
                dispatch(window, event);
            });

            keyboard.impl->hook = addEventHook([this](const Event& event){ keyboard.impl->handle(event); });
            mouse.impl->hook = addEventHook([this](const Event& event){ mouse.impl->handle(event); });

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

        if(!vbo)
        {
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, VertexBufferSize * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
        }
        if(!defaultTexture)
        {
            glGenTextures(1, &defaultTexture);
            glActiveTexture(GL_TEXTURE0);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, defaultTexture);

            Color c = Color::White;
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &c);
        }

        if(engine.impl->modernPipeline)
        {
            glUseProgram(engine.impl->program);

            float left = 0;
            float right = float(width);
            float bottom = float(height);
            float top = 0;
            float near = -1;
            float far = 1;

            GLfloat ortho[16] = {
                2 / (right - left), 0.f, 0.f, 0.f,
                0.f, 2 / (top - bottom), 0.f, 0.f,
                0.f, 0.f, -2 / (far - near), 0.f,
                -(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1.f
            };
            glUniformMatrix4fv(engine.impl->projectionUniform, 1, GL_FALSE, ortho); 
        }
        else
        {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0, width, height, 0, -1, 1);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
        }

        left = (trueWidth - width * scale) / 2;
        bottom = (trueHeight - height * scale) / 2;
        glViewport(left, bottom, width * scale, height * scale);
        glLineWidth(float(scale));

        glScissor(left + clipX * scale, bottom + (height - 1 - clipY2) * scale, (clipX2 - clipX + 1) * scale, (clipY2 - clipY + 1) * scale);
        glEnable(GL_SCISSOR_TEST);
        glDisable(GL_DEPTH_TEST);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        this->trueWidth = trueWidth;
        this->trueHeight = trueHeight;
        interrupt = false;
    }

    void Screen::setResolution(int width, int height, int scale, bool win)
    {
        impl->width = width;
        impl->height = height;
        impl->scale = scale;
        impl->clipX2 = width - 1;
        impl->clipY2 = height - 1;

        impl->resize(width * scale, height * scale, win);
    }

    void Screen::bindImage(Image& image)
    {
        glfwMakeContextCurrent(impl->window);
        image.bindRaw();
    }

    void Screen::unbindImage()
    {
        glfwMakeContextCurrent(impl->window);
    }

    void Screen::applyTransform()
    {
        glfwMakeContextCurrent(impl->window);

        auto& e(impl->engine.impl);
        if(e->modernPipeline)
        {
            glUniform2f(e->originUniform, 0.f, 0.f);
            glUniform2f(e->pivotUniform, 0.f, 0.f);
            glUniform2f(e->scaleUniform, 1.f, 1.f);
            glUniform1f(e->angleUniform, 0.f);
            glUniform4f(e->colorUniform, 1.f, 1.f, 1.f, getOpacity() / 255.f);
        }
        else
        {
            glLoadIdentity();
            glColor4f(1.f, 1.f, 1.f, getOpacity() / 255.f);
        }
        impl->useHardwareBlender(BlendMode::Preserve);
    }

    void Screen::applyTransform(const Transform& transform, int x, int y, int width, int height)
    {
        glfwMakeContextCurrent(impl->window);

        uint8_t r, g, b, a;
        transform.tint.channels(r, g, b, a);

        auto& e(impl->engine.impl);
        if(e->modernPipeline)
        {
            glUniform2f(e->originUniform, float(x), float(y));
            glUniform2f(e->pivotUniform, float(width) / 2, float(height) / 2);
            glUniform2f(e->scaleUniform, float(transform.scaleX * (1 - transform.mirror * 2)), float(transform.scaleY));
            glUniform1f(e->angleUniform, float(transform.angle * M_PI / 180));
            glUniform4f(e->colorUniform, float(r) / 255.f, float(g) / 255.f, float(b) / 255.f, (float(a * getOpacity()) / 255.f) / 255.f);
        }
        else
        {
            glLoadIdentity();
            glTranslated(x, y, 0.0);
            glTranslated(width / 2, height / 2, 0.0);
            glScaled(transform.scaleX * (1 - transform.mirror * 2), transform.scaleY, 0.0);
            glRotated(transform.angle, 0.0, 0.0, 1.0);
            glTranslated(-width / 2, -height / 2, 0.0);
            glColor4f(float(r) / 255.f, float(g) / 255.f, float(b) / 255.f, (float(a * getOpacity()) / 255.f) / 255.f);
        }
        impl->useHardwareBlender(transform.blend);
    }

    void Screen::clear(Color color)
    {
        uint8_t r, g, b, a;
        color.channels(r, g, b, a);
        if(a * getOpacity() / 255 == 255)
        {
            glfwMakeContextCurrent(impl->window);
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
        applyTransform();

        auto& e(impl->engine.impl);
        uint8_t r, g, b, a;
        color.channels(r, g, b, a);
        if(e->modernPipeline)
        {
            glUniform4f(e->colorUniform, float(r) / 255.f, float(g) / 255.f, float(b) / 255.f, (float(a * getOpacity()) / 255.f) / 255.f);
        }
        else
        {
            glColor4f(float(r) / 255.f, float(g) / 255.f, float(b) / 255.f, (float(a * getOpacity()) / 255.f) / 255.f);
        }

        if(x > x2)
        {
            std::swap(x, x2);
        }
        if(y > y2)
        {
            std::swap(y, y2);
        }

        GLfloat vertices[VertexBufferSize] = {
            x - 0.5f, y - 0.5f, 0.f, 0.f,
            x2 + 0.5f, y - 0.5f, 1.f, 0.f,
            x2 + 0.5f, y2 + 0.5f, 1.f, 1.f,
            x2 + 0.5f, y2 + 0.5f, 1.f, 1.f,
            x - 0.5f, y2 + 0.5f, 0.f, 1.f,
            x - 0.5f, y - 0.5f, 0.f, 0.f,
        };

        glBindTexture(GL_TEXTURE_2D, impl->defaultTexture);
        glBindBuffer(GL_ARRAY_BUFFER, impl->vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        if(e->modernPipeline)
        {
            glEnableVertexAttribArray(e->xyAttribute);
            glEnableVertexAttribArray(e->uvAttribute);
            glVertexAttribPointer(e->xyAttribute, 2, GL_FLOAT, false, 4 * sizeof(GLfloat), (void*) 0);
            glVertexAttribPointer(e->uvAttribute, 2, GL_FLOAT, false, 4 * sizeof(GLfloat), (void*)(2 * sizeof(float)));
        }
        else
        {
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glVertexPointer(2, GL_FLOAT, 4 * sizeof(GLfloat), (void*) 0);
            glTexCoordPointer(2, GL_FLOAT, 4 * sizeof(GLfloat), (void*)(2 * sizeof(float)));
        }
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void Screen::grab(int sx, int sy, int sx2, int sy2, int dx, int dy, Canvas& dest)
    {
        glfwMakeContextCurrent(impl->window);

        int x = std::min(std::max(0, std::min(sx, sx2)), impl->width - 1);
        int y = std::min(std::max(0, std::min(sy, sy2)), impl->height - 1);
        int x2 = std::min(std::max(0, std::max(sx, sx2)), impl->width - 1);
        int y2 = std::min(std::max(0, std::max(sy, sy2)), impl->height - 1);

        int w = x2 - x + 1;
        int h = y2 - y + 1;

        int scale = impl->scale;
        int scw = w * scale;
        int sch = h * scale;

        Canvas canvas(scw, sch);
        glReadPixels(impl->left + x * impl->scale, impl->bottom + (impl->height - 1 - y2) * impl->scale, scw, sch, GL_RGBA, GL_UNSIGNED_BYTE, canvas.getData());
        canvas.flip(false, true);

        canvas.scaleBlit<BlendMode::Opaque>(dx, dy, w, h, dest);
    }
}
