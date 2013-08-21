#ifndef PLUM_IMAGE_H
#define PLUM_IMAGE_H

#include <string>
#include "color.h"
#include "screen.h"
#include "canvas.h"
#include "blending.h"

namespace plum
{
    class Canvas;
    class Sheet;
    struct Transform;

    class Image
    {
        public:
            Image(const Canvas& source);
            ~Image();

            Canvas& canvas();
            const Canvas& canvas() const;

            void draw(int x, int y, Screen& dest);
            void draw(int x, int y, const Transform& transform, Screen& dest);
            void drawFrame(const Sheet& sheet, int f, int x, int y, Screen& dest);
            void drawFrame(const Sheet& sheet, int f, int x, int y, const Transform& transform, Screen& dest);

            void bindRaw();
            void drawRaw(int x, int y, Screen& dest);
            void drawFrameRaw(const Sheet& sheet, int f, int x, int y, Screen& dest);

            class Impl;
            std::shared_ptr<Impl> impl;
    };
}

#endif