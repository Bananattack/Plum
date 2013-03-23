#ifndef PLUM_TILEMAP_H
#define PLUM_TILEMAP_H
#include "color.h"
#include "blending.h"

namespace plum
{
    class Sheet;
    class Image;
    class Tilemap
    {
        public:
            static const unsigned int InvalidTile = (unsigned int)(-1);

            Tilemap(int width, int height);
            ~Tilemap();

            int getWidth() const;
            int getHeight() const;

            void clear(unsigned int tileIndex);

            unsigned int getTile(int tx, int ty) const;
            void setTile(int tx, int ty, unsigned int tileIndex);
            void rect(int tx, int ty, int tx2, int ty2, unsigned int tileIndex);
            void solidRect(int tx, int ty, int tx2, int ty2, unsigned int tileIndex);
            void line(int tx, int ty, int tx2, int ty2, unsigned int tileIndex);
            void stamp(int tx, int ty, Tilemap* dest);
            void draw(Image& img, const Sheet& sheet, int worldX, int worldY, int destX, int destY, int tilesWide, int tilesHigh);

        private:
            int width, height;
            unsigned int* data;
    };
}

#endif