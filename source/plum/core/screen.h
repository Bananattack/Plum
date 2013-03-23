#ifndef PLUM_VIDEO_H
#define PLUM_VIDEO_H

#include <string>
#include "color.h"
#include "blending.h"

namespace plum
{
    void useHardwareBlender(BlendMode mode);
    void useHardwareColor(int r, int g, int b, int a);

    class Image;
    class Engine;
    struct Transform;
    class Screen
    {
        public:
            Screen(Engine& engine, int width, int height, int scale, bool win);
            ~Screen();

            int getWidth() const;
            int getHeight() const;
            int getTrueWidth() const;
            int getTrueHeight() const;
            void setTitle(const std::string& title);
            void setResolution(int width, int height, int scale, bool win);

            void bind(Image& image);
            void bind(const Transform& transform, int x, int y, int width, int height);
            void bind(Image& image, const Transform& transform, int x, int y, int width, int height);
            void unbind();

            void clear(Color color);
            void setPixel(int x, int y, Color color, BlendMode mode = BlendPreserve);
            void line(int x, int y, int x2, int y2, Color color, BlendMode mode = BlendPreserve);
            void rect(int x, int y, int x2, int y2, Color color, BlendMode mode = BlendPreserve);
            void solidRect(int x, int y, int x2, int y2, Color color, BlendMode mode = BlendPreserve);
            void horizontalGradientRect(int x, int y, int x2, int y2, Color color, Color color2, BlendMode mode = BlendPreserve);
            void verticalGradientRect(int x, int y, int x2, int y2, Color color, Color color2, BlendMode mode = BlendPreserve);
            void circle(int x, int y, int horizontalRadius, int verticalRadius, Color color, BlendMode mode = BlendPreserve);
            void solidCircle(int x, int y, int horizontalRadius, int verticalRadius, Color color, BlendMode mode = BlendPreserve);

            class Impl;
            std::shared_ptr<Impl> impl;
    };
}

#endif