#pragma once
#include "color.h"
#include "blending.h"

namespace plum
{
    class Spritesheet;
    class Tilemap
    {
        public:
            static const unsigned int InvalidTile = (unsigned int)(-1);

            int width;
            int height;

            unsigned int* data;
            
            // Used for visual display of tilemaps.
            Spritesheet* spr;

            Tilemap(int width, int height);
            ~Tilemap();

            void clear(unsigned int tileIndex);

            int getWidth();
            int getHeight();

            unsigned int getTile(int tx, int ty);
            void setTile(int tx, int ty, unsigned int tileIndex);
            void rect(int tx, int ty, int tx2, int ty2, unsigned int tileIndex);
            void solidRect(int tx, int ty, int tx2, int ty2, unsigned int tileIndex);
            void line(int tx, int ty, int tx2, int ty2, unsigned int tileIndex);
            void stamp(int tx, int ty, Tilemap* dest);
            void blit(int worldX, int worldY, int destX, int destY, int tilesWide, int tilesHigh, BlendMode mode = BlendAlpha, Color tint = Color::White);
    };
}