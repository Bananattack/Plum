#include "canvas.h"
#include "sprite.h"
#include "transform.h"

namespace plum
{
    Sprite::Sprite(const Image& image, int w, int h)
        : image_(image),
        frameWidth(w),
        frameHeight(h),
        padding(0),
        columns(image.canvas().getWidth() / w)
    {
    }
            
    Sprite::~Sprite()
    {
    }

    int Sprite::getFrameWidth() const
    {
        return frameWidth;
    }

    void Sprite::setFrameWidth(int value)
    {
        frameWidth = value;
    }

    int Sprite::getFrameHeight() const
    {
        return frameHeight;
    }

    void Sprite::setFrameHeight(int value)
    {
        frameHeight = value;
    }

    int Sprite::getPadding() const
    {
        return padding;
    }

    void Sprite::setPadding(int value)
    {
        padding = value;
    }

    int Sprite::getColumns() const
    {
        return columns;
    }

    void Sprite::setColumns(int value)
    {
        columns = value;
    }

    Image& Sprite::image()
    {
        return image_;
    }

    void Sprite::bind()
    {
        image_.bind();
    }

    Color Sprite::getFramePixel(int f, int x, int y)
    {
        if(!columns || x < 0 || x >= frameWidth || y < 0 || y >= frameHeight) return 0;

        int fx = (f % columns) * (frameWidth + padding) + padding;
        int fy = (f / columns) * (frameHeight + padding) + padding;
        return image_.canvas().get(fx + x, fy + y);
    }

    void Sprite::blitFrame(int x, int y, int f, BlendMode mode)
    {
        if(!columns) return;

        int fx = (f % columns) * (frameWidth + padding) + padding;
        int fy = (f / columns) * (frameHeight + padding) + padding;
        image_.blitRegion(fx, fy, fx + frameWidth - 1, fy + frameHeight - 1, x, y, mode);
    }

    void Sprite::transformBlitFrame(Transform* transform, int f)
    {
        if(!columns) return;

        Rect* old = transform->clip;
        Rect clip(
            (f % columns) * (frameWidth + padding) + padding,
            (f / columns) * (frameHeight + padding) + padding,
            frameWidth,
            frameHeight
        );

        transform->clip = &clip;
        image_.transformBlit(transform);
        transform->clip = old;
    }

    void Sprite::rawBlitFrame(int x, int y, int f, double angle, double scale)
    {
        if(!columns) return;

        int fx = (f % columns) * (frameWidth + padding) + padding;
        int fy = (f / columns) * (frameHeight + padding) + padding;
        image_.rawBlitRegion(fx, fy, fx + frameWidth - 1, fy + frameHeight - 1, x, y, 0, 1);
    }
}