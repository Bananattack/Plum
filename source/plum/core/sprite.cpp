#include "image.h"
#include "sheet.h"
#include "sprite.h"

namespace plum
{
    Sprite::Sprite()
    {
    }

    Sprite::~Sprite()
    {
    }

    size_t Sprite::getSize() const
    {
        return parts.size();
    }

    size_t Sprite::getCapacity() const
    {
        return parts.capacity();
    }

    void Sprite::setSize(size_t value)
    {
        parts.resize(value);
    }

    void Sprite::setCapacity(size_t value)
    {
        parts.reserve(value);
    }

    void Sprite::add(int x, int y, int frame, const Transform& transform)
    {
        parts.push_back(Part(x, y, frame, transform));
    }

    void Sprite::remove(size_t index)
    {
        if(0 <= index && index < parts.size())
        {
            parts.erase(parts.begin() + index);
        }
    }

    bool Sprite::get(size_t index, int& x, int& y, int& frame, Transform& transform) const
    {
        if(0 <= index && index < parts.size())
        {
            const Part& p(parts[index]);
            x = p.x;
            y = p.y;
            frame = p.frame;
            transform = p.transform;
            return true;
        }
        return false;
    }

    void Sprite::set(size_t index, int x, int y, int frame, const Transform& transform)
    {
        if(0 <= index && index < parts.size())
        {
            parts[index] = Part(x, y, frame, transform);
        }
    }

    void Sprite::draw(Image& image, const Sheet& sheet, int x, int y, Screen& dest) const
    {
        dest.bindImage(image);

        for(const auto& part : parts)
        {
            dest.bindTransform(part.transform, part.x + x, part.y + y, sheet.getWidth(), sheet.getHeight());
            image.drawFrameRaw(sheet, part.frame, 0, 0);
            dest.unbindTransform();
        }

        dest.unbindImage();
    }
}