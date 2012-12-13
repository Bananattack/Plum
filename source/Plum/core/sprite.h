#ifndef PLUM_SPRITE_H
#define PLUM_SPRITE_H

#include "color.h"
#include "blending.h"
#include "image.h"

namespace plum
{
    class Canvas;
    class Sprite
    {
        public:
            Sprite(const Canvas& canvas, int w, int h);
            ~Sprite();

            int getFrameWidth() const;
            int getFrameHeight() const;
            int getPadding() const;
            int getColumns() const;

            void setFrameWidth(int value);
            void setFrameHeight(int value);
            void setPadding(int value);
            void setColumns(int value);

            Image& image();

            void bind();
            Color getFramePixel(int f, int x, int y);
            void blitFrame(int x, int y, int f, BlendMode mode = BlendPreserve, Color tint = Color::White);
            void rawBlitFrame(int x, int y, int f, double angle, double scale);

        private:
            Image image_;
            int frameWidth, frameHeight;
            int padding;
            int columns;
    };
}

#endif