#ifndef PLUM_VIDEO_H
#define PLUM_VIDEO_H

#include <string>
#include "color.h"
#include "blending.h"

struct SDL_Surface;
namespace plum
{
    void useHardwareBlender(BlendMode mode);
    void useHardwareColor(int r, int g, int b, int a);

    class Engine;
    class Video
    {
        public:
            Video(Engine& engine, int width, int height, bool win);
            ~Video();

            int getScreenWidth() const;
            int getScreenHeight() const;
            int getWindowWidth() const;
            int getWindowHeight() const;
            void setTitle(const std::string& title);
            void setResolution(int width, int height, bool win);

            void clear(Color color);
            void setPixel(int x, int y, Color color, BlendMode mode = BlendAlpha);
            void line(int x, int y, int x2, int y2, Color color, BlendMode mode = BlendAlpha);
            void rect(int x, int y, int x2, int y2, Color color, BlendMode mode = BlendAlpha);
            void solidRect(int x, int y, int x2, int y2, Color color, BlendMode mode = BlendAlpha);
            void horizontalGradientRect(int x, int y, int x2, int y2, Color color, Color color2, BlendMode mode = BlendAlpha);
            void verticalGradientRect(int x, int y, int x2, int y2, Color color, Color color2, BlendMode mode = BlendAlpha);
            void circle(int x, int y, int horizontalRadius, int verticalRadius, Color color, BlendMode mode = BlendAlpha);
            void solidCircle(int x, int y, int horizontalRadius, int verticalRadius, Color color, BlendMode mode = BlendAlpha);

        private:
            class Impl;
            std::shared_ptr<Impl> impl;
    };
}

#endif