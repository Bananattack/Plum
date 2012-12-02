#pragma once

#include "color.h"
#include "blending.h"

namespace plum
{
    class Canvas;
    class Image;
    class Spritesheet
    {
        private:
            Image* image;
        public:
            int frameWidth, frameHeight;
            int padding;
            int columns;

            Spritesheet(const Canvas& canvas, int w, int h);
            ~Spritesheet();

        public:
            Image* getImage();
            void bind();
            Color getFramePixel(int f, int x, int y);
            void blitFrame(int x, int y, int f, BlendMode mode = BlendAlpha, Color tint = Color::White);
            void rawBlitFrame(int x, int y, int f, double angle, double scale);
    };
}