#include <cmath>
#include <cstdlib>
#include "tilemap.h"

namespace plum
{
    int Tilemap::getWidth() const
    {
        return width;
    }

    int Tilemap::getHeight() const
    {
        return height;
    }

    void Tilemap::clear(unsigned int tileIndex)
    {
        modified = true;
        for(int i = 0; i < width * height; ++i)
        {
            data[i] = tileIndex;
        }
    }

    unsigned int Tilemap::get(int tx, int ty) const
    {
        if(tx < 0 || tx >= width || ty < 0 || ty >= height) return InvalidTile;
        return data[ty * width + tx];
    }

    void Tilemap::dot(int tx, int ty, unsigned int tileIndex)
    {
        if(tx < 0 || tx >= width || ty < 0 || ty >= height) return;

        modified = true;
        data[ty * width + tx] = tileIndex;
    }

    void Tilemap::rect(int tx, int ty, int tx2, int ty2, unsigned int tileIndex)
    {
        int i;
        if(tx > tx2)
        {
            std::swap(tx, tx2);
        }
        if(ty > ty2)
        {
            std::swap(ty, ty2);
        }
        // Don't draw if completely outside region.
        if(tx >= width || ty >= height || tx2 < 0 || ty2 < 0)
        {
            return;
        }

        modified = true;

        // Keep rectangle inside region
        if(tx < 0)
        {
            tx = 0;
        }
        if(tx2 >= width)
        {
            tx2 = width - 1;
        }
        if(ty < 0)
        {
            ty = 0;
        }
        if(ty2 >= width)
        {
            ty2 = width - 1;
        }
        // Draw the horizontal lines of the rectangle.
        for(i = tx; i <= tx2; ++i)
        {
            data[ty * width + i] = tileIndex;
            data[ty2 * width + i] = tileIndex;
        }
        // Draw the vertical lines of the rectangle.
        for(i = ty; i <= ty2; ++i)
        {
            data[i * width + tx] = tileIndex;
            data[i * width + tx2] = tileIndex;
        }
    }

    void Tilemap::fillRect(int tx, int ty, int tx2, int ty2, unsigned int tileIndex)
    {
        int i, j;
        if(tx > tx2)
        {
            std::swap(tx, tx2);
        }
        if(ty > ty2)
        {
            std::swap(ty, ty2);
        }
        // Don't draw if completely outside region.
        if(tx >= width || ty >= height || tx2 < 0 || ty2 < 0)
        {
            return;
        }

        modified = true;

        // Keep rectangle inside region
        if(tx < 0)
        {
            tx = 0;
        }
        if(tx2 >= width)
        {
            tx2 = width - 1;
        }
        if(ty < 0)
        {
            ty = 0;
        }
        if(ty2 >= width)
        {
            ty2 = width - 1;
        }
        // Plot the solid rectangle
        for(i = ty; i <= ty2; ++i)
        {
            for(j = tx; j <= tx2; ++j)
            {
                data[i * width + j] = tileIndex;
            }
        }
    }

    void Tilemap::line(int tx, int ty, int tx2, int ty2, unsigned int tileIndex)
    {
        // Cohen-Sutherland clipping implementation used here originally by Andy Friesen.
        // Used with permission.
        int c1 = 0;
        int c2 = 0;

        // Compute all the region codes for this line.
        if(tx < 0)    c1 |= 1;
        if(tx >= width)    c1 |= 2;
        if(ty < 0)    c1 |= 4;
        if(ty >= height)    c1 |= 8;
        if(tx2 < 0)    c2 |= 1;
        if(tx2 >= width)    c2 |= 2;
        if(ty2 < 0)    c2 |= 4;
        if(ty2 >= height)    c2 |= 8;

        // Keep clipping until we either accept or reject this line.
        while((c1 & c2) == 0 && (c1 | c2) != 0)
        {
            if(c1)
            {
                // Clip left.
                if(c1 & 1)
                {
                    ty -= ((ty - ty2) * (0 - tx)) / (tx2 - tx + 1);
                    tx = 0;
                }
                // Clip right.
                if(c1 & 2)
                {
                    ty -= ((ty - ty2) * (tx - width - 1)) / (tx2 - tx + 1);
                    tx = width - 1;
                }
                // Clip above.
                else if(c1 & 4)
                {
                    tx -= ((tx - tx2) * (0 - ty)) / (ty2 - ty + 1);
                    ty = 0;
                }
                // Clip below.
                else
                {
                    tx -= ((tx - tx2) * (ty - height - 1)) / (ty2 - ty + 1);
                    ty = height - 1;
                }
                // Recheck all the region codes for this side of the line.
                c1 = 0;
                if(tx < 0)    c1 |= 1;
                if(tx >= width)    c1 |= 2;
                if(ty < 0)    c1 |= 4;
                if(ty >= height)    c1 |= 8;
            }
            else
            {
                // Clip left.
                if(c2 & 1)
                {
                    ty2 -= ((ty2 - ty) * (0 - tx2)) / (tx - tx2 + 1);
                    tx2 = 0;
                }
                // Clip right.
                if(c2 & 2)
                {
                    ty2 -= ((ty2 - ty) * (tx2 - width - 1)) / (tx - tx2 + 1);
                    tx2 = width - 1;
                }
                // Clip above.
                else if(c2 & 4)
                {
                    tx2 -= ((tx2 - tx) * (0 - ty2)) / (ty - ty2 + 1);
                    ty2 = 0;
                }
                // Clip below.
                else
                {
                    tx2 -= ((tx2 - tx) * (ty2 - height - 1)) / (ty - ty2 + 1);
                    ty2 = height;
                }
                // Recheck all the region codes for this side of the line.
                c2 = 0;
                if(tx2 < 0)    c2 |= 1;
                if(tx2 >= width)    c2 |= 2;
                if(ty2 < 0)    c2 |= 4;
                if(ty2 >= height)    c2 |= 8;
            }
        }
        // Rejected.
        if(c1 & c2)
        {
            return;
        }

        modified = true;

        // A single pixel
        if(tx == tx2 && ty == ty2)
        {
            data[ty * width + tx] = tileIndex;
            return;
        }
        // Horizontal line
        if(ty == ty2)
        {
            // Put the coordinates in order.
            if(tx > tx2)
            {
                std::swap(tx, tx2);
            }
            // Draw it.
            for(int i = tx; i <= tx2; ++i)
            {
                data[ty * width + i] = tileIndex;
            }
            return;
        }
        // Vertical line
        if(tx == tx2)
        {
            // Put the coordinates in order.
            if(ty > ty2)
            {
                std::swap(ty, ty2);
            }
            // Draw it.
            for(int i = ty; i <= ty2; ++i)
            {
                data[i * width + tx] = tileIndex;
            }
            return;
        }

        // Bresenham line-drawing implementation used here originally by Shamus Peveril.
        // Used with permission.
        int cx, cy, dx, dy;
        int xaccum, yaccum, xincre, yincre, xreset, yreset, xchange, ychange;
        bool done = false;

        dx = std::llabs(tx2 - tx);
        dy = std::llabs(ty2 - ty);
        cx = tx;
        cy = ty;
        xchange = (tx > tx2) ? -1 : 1;
        ychange = (ty > ty2) ? -1 : 1;

        if(dx > dy)
        {
            xaccum = 0;
            xreset = 0;
            xincre = 0;
            yaccum = dy * 2 - dx;
            yincre = dy * 2;
            yreset = (dy - dx) * 2;
        }
        else
        {
            yaccum = 0;
            yreset = 0;
            yincre = 0;
            xaccum = dx * 2 - dy;
            xincre = dx * 2;
            xreset = (dx - dy) * 2;
        }

        while(!done)
        {
            if(xaccum < 0)
            {
                xaccum += xincre;
            }
            else
            {
                cx += xchange;
                xaccum += xreset;
            }

            if(yaccum < 0)
            {
                yaccum += yincre;
            }
            else
            {
                cy += ychange;
                yaccum += yreset;
            }

            data[cy * width + cx] = tileIndex;

            if(xreset == 0 && cx == tx2) done = true;
            if(yreset == 0 && cy == ty2) done = true;
        }
    }

    void Tilemap::stamp(int tx, int ty, Tilemap* dest)
    {
        int i, j;
        int tx2 = tx + width - 1;
        int ty2 = ty + height - 1;
        int sourceX = 0;
        int sourceY = 0;
        int sourceX2 = width - 1;
        int sourceY2 = height - 1;

        // Don't plot if completely outside dest region.
        if(tx >= dest->width || ty >= dest->height || tx2 < 0 || ty2 < 0)
        {
            return;
        }

        modified = true;

        // Keep rectangle inside dest region
        if(tx < 0)
        {
            sourceX += 0 - tx;
        }
        if(tx2 >= dest->width)
        {
            sourceX2 -= tx2 - dest->width - 1;
        }
        if(ty < 0)
        {
            sourceY += 0 - ty;
        }
        if(tx2 >= dest->height)
        {
            sourceY2 -= ty2 - dest->height - 1;
        }
        // Plot the tilemap, tile for tile
        for(i = sourceY; i <= sourceY2; ++i)
        {
            for(j = sourceX; j <= sourceX2; ++j)
            {
                dest->data[(i + ty) * dest->width + (j + tx)] = data[i * width + j];
            }
        }
    }
}
