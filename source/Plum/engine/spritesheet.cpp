#pragma once

#include "spritesheet.h"


#include "../video/canvas.h"
#include "../video/image.h"

namespace plum
{
    Spritesheet::Spritesheet(Canvas* canvas, int w, int h)
    {
        init(canvas, w, h);
    }

    Spritesheet::Spritesheet(Image* img, int w, int h)
    {
        init(img->getCanvas(), w, h);
    }
            
    Spritesheet::~Spritesheet()
    {
        delete image;
    }

    void Spritesheet::init(Canvas* canvas, int w, int h)
    {
        image = new Image(canvas);
        frameWidth = w;
        frameHeight = h;
        padding = 0;
        columns = image->getCanvas()->getWidth() / w;
    }

    Image* Spritesheet::getImage()
    {
        return image;
    }

    void Spritesheet::bind()
    {
        image->bind();
    }

    Color Spritesheet::getFramePixel(int f, int x, int y)
    {
        if(!columns || x < 0 || x >= frameWidth || y < 0 || y >= frameHeight) return 0;

        int fx = (f % columns) * (frameWidth + padding) + padding;
        int fy = (f / columns) * (frameHeight + padding) + padding;
        return image->getCanvas()->getPixel(fx + x, fy + y);
    }

    void Spritesheet::blitFrame(int x, int y, int f, BlendMode mode, Color tint)
    {
        if(!columns) return;

        int fx = (f % columns) * (frameWidth + padding) + padding;
        int fy = (f / columns) * (frameHeight + padding) + padding;
        image->blitRegion(fx, fy, fx + frameWidth - 1, fy + frameHeight - 1,
                x, y, mode, tint);
    }

    void Spritesheet::rawBlitFrame(int x, int y, int f, double angle, double scale)
    {
        if(!columns) return;

        int fx = (f % columns) * (frameWidth + padding) + padding;
        int fy = (f / columns) * (frameHeight + padding) + padding;
        image->rawBlitRegion(fx, fy, fx + frameWidth - 1, fy + frameHeight - 1,
                x, y, 0, 1);
    }
}