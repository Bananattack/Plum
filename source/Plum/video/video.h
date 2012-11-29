#pragma once

#include <string>
#include "color.h"
#include "blending.h"

struct SDL_Surface;
namespace plum
{
    void useHardwareBlender(BlendMode mode);
    void useHardwareColor(int r, int g, int b, int a);

    class Video
    {
        public:
            static Video* create(int width, int height, bool win);

            virtual ~Video() {}

            virtual void setResolution(int width, int height, bool win) = 0;
            virtual int getScreenWidth() const = 0;
            virtual int getScreenHeight() const = 0;
            virtual int getWindowWidth() const = 0;
            virtual int getWindowHeight() const = 0;

            virtual void clear(Color color) = 0;
            virtual void setPixel(int x, int y, Color color, BlendMode mode = BlendAlpha) = 0;
            virtual void line(int x, int y, int x2, int y2, Color color, BlendMode mode = BlendAlpha) = 0;
            virtual void rect(int x, int y, int x2, int y2, Color color, BlendMode mode = BlendAlpha) = 0;
            virtual void solidRect(int x, int y, int x2, int y2, Color color, BlendMode mode = BlendAlpha) = 0;
            virtual void horizontalGradientRect(int x, int y, int x2, int y2, Color color, Color color2, BlendMode mode = BlendAlpha) = 0;
            virtual void verticalGradientRect(int x, int y, int x2, int y2, Color color, Color color2, BlendMode mode = BlendAlpha) = 0;
            virtual void circle(int x, int y, int horizontalRadius, int verticalRadius, Color color, BlendMode mode = BlendAlpha) = 0;
            virtual void solidCircle(int x, int y, int horizontalRadius, int verticalRadius, Color color, BlendMode mode = BlendAlpha) = 0;
    };
}

