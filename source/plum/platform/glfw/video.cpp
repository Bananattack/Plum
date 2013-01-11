#include <cmath>
#include <GL/glfw3.h>

#include "engine.h"
#include "../../core/video.h"

namespace plum
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

    void useHardwareColor(int r, int g, int b, int a) 
    {
        return glColor4ub(r, g, b, a * getOpacity() / 255);
    }

    class Screen::Impl
    {
        public:
            Impl(Engine& engine)
                : engine(engine)
            {
                hook = engine.addUpdateHook([this](){ update(); });
            }

            ~Impl()
            {
            }

            void update()
            {
                glfwSwapBuffers(context->window());
            }

            Engine& engine;
            std::shared_ptr<Engine::UpdateHook> hook;
            std::shared_ptr<WindowContext> context;

            bool windowed;

            int trueWidth, trueHeight;
            int width, height;
    };

    Screen::Screen(Engine& engine, int width, int height, bool win)
        : impl(new Impl(engine))
    {
        setResolution(width, height, win);
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

    void Screen::setTitle(const std::string& title)
    {
        glfwSetWindowTitle(impl->context->window(), title.c_str());
    }

    void Screen::setResolution(int width, int height, bool win)
    {
        impl->windowed = win;

        impl->width = width;
        impl->height = height;

        // Fullscreen is usually not supported for low-res, so upscale everything!
        if(!impl->windowed && (width < 640 || height < 480))
        {
            width = 640;
            height = 480;
        }

        auto window = glfwCreateWindow(width, height, (impl->windowed ? GLFW_WINDOWED : GLFW_FULLSCREEN), "", nullptr);
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

        glDisable(GL_DEPTH_TEST);

        glScissor(0, 0, impl->width, impl->height);
        glEnable(GL_SCISSOR_TEST);

        glClearColor(0.0, 0.0, 0.0, 1.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glfwSwapInterval(1);
        glfwShowWindow(window);
        impl->context = impl->engine.impl->registerWindow(window);
    }

    void Screen::startBatch()
    {
        glEnable(GL_TEXTURE_2D);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    void Screen::endBatch()
    {
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    void Screen::clear(Color color)
    {
        uint8_t r, g, b, a;
        color.channels(r, g, b, a);
        glClearColor(r / 255.0f,
                g / 255.0f,
                b / 255.0f,
                a / 255.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    // Dear god I hope nobody uses this for anything intensive.
    void Screen::setPixel(int x, int y, Color color, BlendMode mode)
    {
        uint8_t r, g, b, a;
        color.channels(r, g, b, a);

        useHardwareBlender(mode);

        const GLdouble vertexArray[] = { x, y };
        glDisable(GL_TEXTURE_2D);

        useHardwareColor(r, g, b, a);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_DOUBLE, 0, vertexArray);
        glDrawArrays(GL_POINTS, 0, 1);
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    void Screen::line(int x, int y, int x2, int y2, Color color, BlendMode mode)
    {
        uint8_t r, g, b, a;
        color.channels(r, g, b, a);

        useHardwareBlender(mode);

        const GLdouble vertexArray[] = { x, y - 1, x2 + 1, y2 };
        glDisable(GL_TEXTURE_2D);

        useHardwareColor(r, g, b, a);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_DOUBLE, 0, vertexArray);
        glDrawArrays(GL_LINES, 0, 2);
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    void Screen::rect(int x, int y, int x2, int y2, Color color, BlendMode mode)
    {
        uint8_t r, g, b, a;
        color.channels(r, g, b, a);

        useHardwareBlender(mode);

        if(x > x2)
        {
            std::swap(x, x2);
        }
        if(y > y2)
        {
            std::swap(y, y2);
        }

        const GLdouble vertexArray[] = {
            x - 1, y - 1,
            x - 1, y2,
            x - 1, y2,
            x2 + 1, y2,
            x2 + 1, y2,
            x2 + 2, y - 2,
            x2 + 2, y - 2,
            x - 1, y - 1,
        };
        glDisable(GL_TEXTURE_2D);

        useHardwareColor(r, g, b, a);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_DOUBLE, 0, vertexArray);
        glDrawArrays(GL_LINES, 0, 8);
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    void Screen::solidRect(int x, int y, int x2, int y2, Color color, BlendMode mode)
    {
        uint8_t r, g, b, a;
        color.channels(r, g, b, a);    

        useHardwareBlender(mode);

        if(x > x2)
        {
            std::swap(x, x2);
        }
        if(y > y2)
        {
            std::swap(y, y2);
        }


        const GLdouble vertexArray[] = {
            x - 1, y - 1,
            x2 + 1, y - 1,
            x2 + 1, y2,
            x - 1, y2,
        };
        glDisable(GL_TEXTURE_2D);

        useHardwareColor(r, g, b, a);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_DOUBLE, 0, vertexArray);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    void Screen::horizontalGradientRect(int x, int y, int x2, int y2, Color color, Color color2, BlendMode mode)
    {
        uint8_t r, g, b, a;
        uint8_t r2, g2, b2, a2;
        color.channels(r, g, b, a);
        color2.channels(r2, g2, b2, a2);

        useHardwareBlender(mode);

        if(x > x2)
        {
            std::swap(x, x2);
        }
        if(y > y2)
        {
            std::swap(y, y2);
        }

        const GLdouble vertexArray[] = {
            x - 1, y - 1,
            x2 + 1, y - 1,
            x2 + 1, y2,
            x - 1, y2,
        };
        const uint8_t colorArray[] = {
            r2, g2, b2, a2 * getOpacity() / 255,
            r, g, b, a * getOpacity() / 255,
            r, g, b, a * getOpacity() / 255,
            r2, g2, b2, a2 * getOpacity() / 255,
        };
        glDisable(GL_TEXTURE_2D);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(2, GL_DOUBLE, 0, vertexArray);
        glColorPointer(4, GL_UNSIGNED_BYTE, 0, colorArray);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    void Screen::verticalGradientRect(int x, int y, int x2, int y2, Color color, Color color2, BlendMode mode)
    {
        uint8_t r, g, b, a;
        uint8_t r2, g2, b2, a2;
        color.channels(r, g, b, a);
        color2.channels(r2, g2, b2, a2);

        useHardwareBlender(mode);

        if(x > x2)
        {
            std::swap(x, x2);
        }
        if(y > y2)
        {
            std::swap(y, y2);
        }

        const GLdouble vertexArray[] = {
            x - 1, y - 1,
            x2 + 1, y - 1,
            x2 + 1, y2,
            x - 1, y2,
        };
        const uint8_t colorArray[] = {
            r, g, b, a * getOpacity() / 255,
            r, g, b, a * getOpacity() / 255,
            r2, g2, b2, a2 * getOpacity() / 255,
            r2, g2, b2, a2 * getOpacity() / 255,
        };
        glDisable(GL_TEXTURE_2D);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(2, GL_DOUBLE, 0, vertexArray);
        glColorPointer(4, GL_UNSIGNED_BYTE, 0, colorArray);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    void Screen::circle(int x, int y, int horizontalRadius, int verticalRadius, Color color, BlendMode mode)
    {
        uint8_t r, g, b, a;
        color.channels(r, g, b, a);

        useHardwareBlender(mode);

        double px = x;
        double py = y + verticalRadius;

        GLdouble vertexArray[360 * 2];
        for(int i = 0; i < 360; ++i)
        {
            vertexArray[i * 2] = px;
            vertexArray[i * 2 + 1] = py;
            px = x + (horizontalRadius * (double) sin(i * M_PI / 180.0));
            py = y + (verticalRadius * (double) cos(i * M_PI / 180.0));
        }

        glDisable(GL_TEXTURE_2D);

        useHardwareColor(r, g, b, a);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_DOUBLE, 0, vertexArray);
        glDrawArrays(GL_LINE_LOOP, 0, 360);
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    void Screen::solidCircle(int x, int y, int horizontalRadius, int verticalRadius, Color color, BlendMode mode)
    {
        uint8_t r, g, b, a;
        color.channels(r, g, b, a);

        useHardwareBlender(mode);

        double px = x;
        double py = y + verticalRadius;

        GLdouble vertexArray[360 * 2];
        for(int i = 0; i < 360; ++i)
        {
            vertexArray[i * 2] = px;
            vertexArray[i * 2 + 1] = py;
            px = x + (horizontalRadius * (double) sin(i * M_PI / 180.0));
            py = y + (verticalRadius * (double) cos(i * M_PI / 180.0));
        }

        glDisable(GL_TEXTURE_2D);

        useHardwareColor(r, g, b, a);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_DOUBLE, 0, vertexArray);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 360);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
}
