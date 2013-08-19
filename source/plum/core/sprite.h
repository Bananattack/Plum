#ifndef PLUM_SPRITE_H
#define PLUM_SPRITE_H

#include "transform.h"

#include <vector>

namespace plum
{
    class Sheet;
    class Image;
    class Screen;
    class Sprite
    {
        public:
            Sprite();
            ~Sprite();

            size_t getSize() const;
            size_t getCapacity() const;
            void setSize(size_t value);
            void setCapacity(size_t value);

            void add(int x, int y, int frame, const Transform& transform);
            void remove(size_t index);
            bool get(size_t index, int& x, int& y, int& frame, Transform& transform) const;
            void set(size_t index, int x, int y, int frame, const Transform& transform);
            void draw(Image& image, const Sheet& sheet, int x, int y, Screen& dest) const;

        private:
            struct Part
            {
                int x;
                int y;
                int frame;
                Transform transform;

                Part()
                    : x(-214783648), y(-214783648), frame(0)
                {
                }

                Part(int x, int y, int frame, const Transform& transform)
                    : x(x), y(y), frame(frame), transform(transform)
                {
                }
            };

            std::vector<Part> parts;
    };
}

#endif