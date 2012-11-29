#include <cmath>

#include <SDL.h>
#include <SDL_opengl.h>

#include "../video/video.h"

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
            case BlendAlpha:
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

    class SDLVideo : public Video
    {
        private:
            bool windowed;

            int windowWidth, windowHeight;
            int xres, yres;
            int desktopWidth, desktopHeight;

            SDL_Surface* frontSurface;
        public:
            SDLVideo(int width, int height, bool win);
            ~SDLVideo() {}

            void setResolution(int width, int height, bool win);

            int getScreenWidth() const
            {
                return xres;
            }

            int getScreenHeight() const
            {
                return yres;
            }

            int getWindowWidth() const
            {
                return windowWidth;
            }

            int getWindowHeight() const
            {
                return windowHeight;
            }

            void clear(Color color);
            void setPixel(int x, int y, Color color, BlendMode mode = BlendAlpha);
            void line(int x, int y, int x2, int y2, Color color, BlendMode mode = BlendAlpha);
            void rect(int x, int y, int x2, int y2, Color color, BlendMode mode = BlendAlpha);
            void solidRect(int x, int y, int x2, int y2, Color color, BlendMode mode = BlendAlpha);
            void horizontalGradientRect(int x, int y, int x2, int y2, Color color, Color color2, BlendMode mode = BlendAlpha);
            void verticalGradientRect(int x, int y, int x2, int y2, Color color, Color color2, BlendMode mode = BlendAlpha);
            void circle(int x, int y, int horizontalRadius, int verticalRadius, Color color, BlendMode mode = BlendAlpha);
            void solidCircle(int x, int y, int horizontalRadius, int verticalRadius, Color color, BlendMode mode = BlendAlpha);
    };

    Video* Video::create(int width, int height, bool win)
    {
        return new SDLVideo(width, height, win);
    }

    SDLVideo::SDLVideo(int width, int height, bool win)
    {
        const SDL_VideoInfo* vidinfo = SDL_GetVideoInfo();
        desktopWidth = vidinfo->current_w;
        desktopHeight = vidinfo->current_h;
        setResolution(width, height, win);
    }

    void SDLVideo::setResolution(int width, int height, bool win)
    {
        windowed = win;

        xres = width;
        yres = height;

        // Fullscreen is usually not supported for low-res, so upscale everything!
        if(!windowed && (width < 640 || height < 480))
        {
            width = 640;
            height = 480;
        }

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        frontSurface = SDL_SetVideoMode(width, height, 32, (windowed ? 0 : SDL_FULLSCREEN) | SDL_OPENGL);
        if(!frontSurface)
        {
            throw std::runtime_error("Video settings were not compatible your graphics card.\r\n");
        }
        windowWidth = frontSurface->w;
        windowHeight = frontSurface->h;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, xres, yres, 0, -1, 1);

        glDisable(GL_DEPTH_TEST);

        glScissor(0, 0, xres, yres);
        glEnable(GL_SCISSOR_TEST);

        glClearColor(0.0, 0.0, 0.0, 1.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    void SDLVideo::clear(Color color)
    {
        ColorChannel r, g, b, a;
        getRGBA(color, r, g, b, a);
        glClearColor(r / 255.0f,
                g / 255.0f,
                b / 255.0f,
                a / 255.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    // Dear god I hope nobody uses this for anything intensive.
    void SDLVideo::setPixel(int x, int y, Color color, BlendMode mode)
    {
        ColorChannel r, g, b, a;
        getRGBA(color, r, g, b, a);

        useHardwareBlender(mode);

        GLdouble vertexArray[] = { x, y };
        glDisable(GL_TEXTURE_2D);

        useHardwareColor(r, g, b, a);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_DOUBLE, 0, vertexArray);
        glDrawArrays(GL_POINTS, 0, 1);
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    void SDLVideo::line(int x, int y, int x2, int y2, Color color, BlendMode mode)
    {
        ColorChannel r, g, b, a;
        getRGBA(color, r, g, b, a);

        useHardwareBlender(mode);

        GLdouble vertexArray[] = { x, y - 1, x2 + 1, y2 };
        glDisable(GL_TEXTURE_2D);

        useHardwareColor(r, g, b, a);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_DOUBLE, 0, vertexArray);
        glDrawArrays(GL_LINES, 0, 2);
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    void SDLVideo::rect(int x, int y, int x2, int y2, Color color, BlendMode mode)
    {
        ColorChannel r, g, b, a;
        getRGBA(color, r, g, b, a);

        useHardwareBlender(mode);

        if(x > x2)
        {
            std::swap(x, x2);
        }
        if(y > y2)
        {
            std::swap(y, y2);
        }

        glDisable(GL_TEXTURE_2D);
        glPushMatrix();
        glBegin(GL_LINES);
            useHardwareColor(r, g, b, a);
            glVertex2d(x - 1, y - 1);
            glVertex2d(x - 1, y2);
            glVertex2d(x - 1, y2);
            glVertex2d(x2 + 1, y2);
            glVertex2d(x2 + 1, y2);
            glVertex2d(x2 + 2, y - 2);
            glVertex2d(x2 + 2, y - 2);
            glVertex2d(x - 1, y - 1);
        glEnd();
    
        glPopMatrix();
    }


    void SDLVideo::solidRect(int x, int y, int x2, int y2, Color color, BlendMode mode)
    {
        ColorChannel r, g, b, a;
        getRGBA(color, r, g, b, a);    

        useHardwareBlender(mode);

        if(x > x2)
        {
            std::swap(x, x2);
        }
        if(y > y2)
        {
            std::swap(y, y2);
        }

        glDisable(GL_TEXTURE_2D);
        glPushMatrix();
        glBegin(GL_QUADS);
            useHardwareColor(r, g, b, a);
            glVertex2d(x - 1, y - 1);
            glVertex2d(x2 + 1, y - 1);
            glVertex2d(x2 + 1, y2);
            glVertex2d(x - 1, y2);
        glEnd();

        glPopMatrix();
    }

    void SDLVideo::horizontalGradientRect(int x, int y, int x2, int y2, Color color, Color color2, BlendMode mode)
    {
        ColorChannel r, g, b, a;
        getRGBA(color, r, g, b, a);

        useHardwareBlender(mode);

        if(x > x2)
        {
            std::swap(x, x2);
        }
        if(y > y2)
        {
            std::swap(y, y2);
        }

        glDisable(GL_TEXTURE_2D);
        glPushMatrix();
        glBegin(GL_QUADS);
            useHardwareColor(r, g, b, a);
            glVertex2d(x - 1, y - 1);
            getRGBA(color2, r, g, b, a);
            useHardwareColor(r, g, b, a);
            glVertex2d(x2 + 1, y - 1);
            getRGBA(color, r, g, b, a);
            useHardwareColor(r, g, b, a);
            glVertex2d(x2 + 1, y2);
            getRGBA(color2, r, g, b, a);
            useHardwareColor(r, g, b, a);
            glVertex2d(x - 1, y2);
        glEnd();

        glPopMatrix();
    }

    void SDLVideo::verticalGradientRect(int x, int y, int x2, int y2, Color color, Color color2, BlendMode mode)
    {
        ColorChannel r, g, b, a;
        getRGBA(color, r, g, b, a);

        useHardwareBlender(mode);

        if(x > x2)
        {
            std::swap(x, x2);
        }
        if(y > y2)
        {
            std::swap(y, y2);
        }

        glDisable(GL_TEXTURE_2D);
        glPushMatrix();
        glBegin(GL_QUADS);
            useHardwareColor(r, g, b, a);
            glVertex2d(x - 1, y - 1);
            glVertex2d(x2 + 1, y - 1);
            getRGBA(color2, r, g, b, a);
            useHardwareColor(r, g, b, a);
            glVertex2d(x2 + 1, y2);
            glVertex2d(x - 1, y2);
        glEnd();

        glPopMatrix();
    }

    void SDLVideo::circle(int x, int y, int horizontalRadius, int verticalRadius, Color color, BlendMode mode)
    {
        ColorChannel r, g, b, a;
        getRGBA(color, r, g, b, a);

        useHardwareBlender(mode);

        double x1 = x;
        double y1 = y + verticalRadius;
        double angle;

        glDisable(GL_TEXTURE_2D);
        glPushMatrix();
        glBegin(GL_LINE_STRIP);            
            useHardwareColor(r, g, b, a);
            for(angle = 0.0; angle <= 2.0 * M_PI; angle += 0.01)
            {                
                glVertex2d(x1, y1);
                x1 = x + (horizontalRadius * (double) sin(angle));
                y1 = y + (verticalRadius * (double) cos(angle));            
            }
        glEnd();
        glPopMatrix();
    }

    void SDLVideo::solidCircle(int x, int y, int horizontalRadius, int verticalRadius, Color color, BlendMode mode)
    {
        ColorChannel r, g, b, a;
        getRGBA(color, r, g, b, a);

        useHardwareBlender(mode);

        double x1 = x;
        double y1 = y + verticalRadius;
        double angle;

        glDisable(GL_TEXTURE_2D);
        glPushMatrix();
        glBegin(GL_TRIANGLES);    
            useHardwareColor(r, g, b, a);
            for(angle = 0.0; angle <= 2.0 * M_PI; angle += 0.01)
            {    
                glVertex2d(x, y);
                glVertex2d(x1, y1);
                x1 = x + (horizontalRadius * (double) sin(angle));
                y1 = y + (verticalRadius * (double) cos(angle));
                glVertex2d(x1, y1);            
            }
        glEnd();
        glPopMatrix();
    }
}
