#ifndef PLUM_CANVAS_H
#define PLUM_CANVAS_H

#include <cmath>
#include <string>
#include <vector>

#include "color.h"
#include "blending.h"

namespace plum
{
    class Canvas
    {
        public:
            static Canvas load(const std::string& filename);

            Canvas()
                : width(0),
                height(0),
                trueWidth(0),
                trueHeight(0),
                clipX(0),
                clipY(0),
                clipX2(0),
                clipY2(0),
                data(nullptr)
            {
            }

            Canvas(int width, int height)
                : width(width),
                height(height),
                trueWidth(width),
                trueHeight(height),
                clipX(0),
                clipY(0),
                clipX2(width - 1),
                clipY2(height - 1),
                data(new Color[width * height])
            {
                clear(Color::Black);
            }

            Canvas(int width, int height, int trueWidth, int trueHeight)
                : width(width),
                height(height),
                trueWidth(trueWidth),
                trueHeight(trueHeight),
                clipX(0),
                clipY(0),
                clipX2(width - 1),
                clipY2(height - 1),
                data(new Color[trueWidth * trueHeight])
            {
                clear(Color::Black);
            }

            Canvas(const Canvas& other)
                : width(other.width),
                height(other.height),
                trueWidth(other.trueWidth),
                trueHeight(other.trueHeight),
                clipX(other.clipX),
                clipY(other.clipY),
                clipX2(other.clipX2),
                clipY2(other.clipY2),
                data(new Color[other.trueWidth * other.trueHeight])
            {
                std::memcpy(data, other.data, sizeof(Color) * other.width * other.height);
            }

            Canvas(Canvas&& other)
                : width(other.width),
                height(other.height),
                trueWidth(other.trueWidth),
                trueHeight(other.trueHeight),
                clipX(other.clipX),
                clipY(other.clipY),
                clipX2(other.clipX2),
                clipY2(other.clipY2),
                data(other.data)
            {
                other.data = nullptr;
            }

            ~Canvas()
            {
                delete[] data;
            }

            Canvas& operator =(const Canvas& other)
            {
                Canvas temp(other);
                swap(temp);
                return *this;
            }

            Canvas& operator =(Canvas&& other)
            {
                swap(other);
                return *this;
            }

            void swap(Canvas& other) throw()
            {
                std::swap(width, other.width);
                std::swap(height, other.height);
                std::swap(trueWidth, other.trueWidth);
                std::swap(trueHeight, other.trueHeight);
                std::swap(clipX, other.clipX);
                std::swap(clipY, other.clipY);
                std::swap(clipX2, other.clipX2);
                std::swap(clipY2, other.clipY2);
                std::swap(data, other.data);
            }

            int getTrueWidth() const
            {
                return trueWidth;
            }

            int getTrueHeight() const
            {
                return trueHeight;
            }

            int getWidth() const
            {
                return width;
            }

            int getHeight() const
            {
                return height;
            }

            Color* getData() const
            {
                return data;
            }

            void getClipRegion(int& x, int& y, int& x2, int& y2) const
            {
                x = clipX;
                y = clipY;
                x2 = clipX2;
                y2 = clipY2;
            }

            void restoreClipRegion()
            {
                clipX = 0;
                clipY = 0;
                clipX2 = width - 1;
                clipY2 = height - 1;    
            }

            void setClipRegion(int x, int y, int x2, int y2)
            {
                if (x > x2)
                {
                    std::swap(x, x2);
                }
                if (y > y2)
                {
                    std::swap(y, y2);
                }
                clipX = std::min(std::max(0, x), width - 1);
                clipY = std::min(std::max(0, y), height - 1);
                clipX2 = std::min(std::max(0, x2), width - 1);
                clipY2 = std::min(std::max(0, y2), height - 1);
            }
            
            Color getPixel(int x, int y) const
            {
                if(!data || x < clipX || x >= clipX2 || y < clipY || y >= clipY2) return 0;
                else return data[y * trueWidth + x];
            }

            template <typename BlendCallback> void setPixel(int x, int y, Color color, const BlendCallback& blend)
            {
                if(!data || x < clipX || x > clipX2 || y < clipY || y > clipY2) return;
                else data[y * trueWidth + x] = blend(color, data[y * trueWidth + x]);
            }

            void clear(Color color)
            {
                if(!data) return;
                for(int i = 0; i < trueWidth * trueHeight; i++)
                {
                    data[i] = color;
                }
            }

            void replaceColor(Color find, Color replacement)
            {
                if(!data) return;
                for(int i = 0; i < trueWidth * trueHeight; i++)
                {
                    if(data[i] == find)
                    {
                        data[i] = replacement;
                    }
                }
            }

            void flip(bool horizontal, bool vertical)
            {
                if(!data) return;
                if(horizontal)
                {
                    for(int x = 0; x < width / 2; x++)
                    {
                        for(int y = 0; y < height; y++)
                        {
                            Color t = data[y * trueWidth + x];
                            data[y * trueWidth + x] = data[y * trueWidth + (width - x - 1)];
                            data[y * trueWidth + (width - x - 1)] = t;
                        }
                    }
                }
                if(vertical)
                {
                    for(int x = 0; x < width; x++)
                    {
                        for(int y = 0; y < height / 2; y++)
                        {
                            Color t = data[y * trueWidth + x];
                            data[y * trueWidth + x] = data[(height - y - 1) * trueWidth + x];
                            data[(height - y - 1) * trueWidth + x] = t;
                        }
                    }
                }
            }

            template <typename BlendCallback> void line(int x, int y, int x2, int y2, Color color, const BlendCallback& blend)
            {
                if(!data) return;
                // Now we'll clip the line using Cohen-Sutherland clipping
                // (this source adapted from ika)
                int c1 = 0;
                int c2 = 0;

                // Silly region code things to start us off
                if(x < clipX) c1 |= 1;
                if(x > clipX2) c1 |= 2;
                if(y < clipY) c1 |= 4;
                if(y > clipY2) c1 |= 8;
                if(x2 < clipX) c2 |= 1;
                if(x2 > clipX2) c2 |= 2;
                if(y2 < clipY) c2 |= 4;
                if(y2 > clipY2) c2 |= 8;

                // Keep clipping until we either accept or reject this line.
                while((c1 & c2) == 0 && (c1 | c2) != 0)
                {
                    if(c1)
                    {
                        // Clip left.
                        if(c1 & 1)
                        {
                            y -= ((y - y2) * (clipX - x)) / (x2 - x + 1);
                            x = clipX;
                        }
                        // Clip right.
                        if(c1 & 2)
                        {
                            y -= ((y - y2) * (x - clipX2)) / (x2 - x + 1);
                            x = clipX2;
                        }
                        // Clip above.
                        else if(c1 & 4)
                        {
                            x -= ((x - x2) * (clipY - y)) / (y2 - y + 1);
                            y = clipY;
                        }
                        // Clip below.
                        else
                        {
                            x -= ((x - x2) * (y - clipY2)) / (y2 - y + 1);
                            y = clipY2;
                        }
                        // Recheck all the region codes for this side of the line.
                        c1 = 0;
                        if(x < clipX) c1 |= 1;
                        if(x > clipX2) c1 |= 2;
                        if(y < clipY) c1 |= 4;
                        if(y > clipY2) c1 |= 8;
                    }
                    else
                    {
                        // Clip left.
                        if(c2 & 1)
                        {
                            y2 -= ((y2 - y) * (clipX - x2)) / (x - x2 + 1);
                            x2 = clipX;
                        }
                        // Clip right.
                        if(c2 & 2)
                        {
                            y2 -= ((y2 - y) * (x2 - clipX2)) / (x - x2 + 1);
                            x2 = clipX2;
                        }
                        // Clip above.
                        else if(c2 & 4)
                        {
                            x2 -= ((x2 - x) * (clipY - y2)) / (y - y2 + 1);
                            y2 = clipY;
                        }
                        // Clip below.
                        else
                        {
                            x2 -= ((x2 - x) * (y2 - clipY2)) / (y - y2 + 1);
                            y2 = clipY2;
                        }
                        // Recheck all the region codes for this side of the line.
                        c2 = 0;
                        if(x2 < clipX) c2 |= 1;
                        if(x2 > clipX2) c2 |= 2;
                        if(y2 < clipY) c2 |= 4;
                        if(y2 > clipY2) c2 |= 8;
                    }
                }
                // Re-jected!
                if(c1 & c2) return;
                // A single pixel
                if(x == x2 && y == y2)
                {
                    data[y * trueWidth + x] = blend(color, data[y * trueWidth + x]);
                    return;
                }
                // Horizontal line
                if(y == y2)
                {
                    // Put the coordinates in order.
                    if (x > x2)
                    {
                        std::swap(x, x2);
                    }
                    // Draw it.
                    for(int i = x; i <= x2; i++)
                    {
                        data[y * trueWidth + i] = blend(color, data[y * trueWidth + i]);
                    }
                    return;
                }
                // Vertical line
                if(x == x2)
                {
                    // Put the coordinates in order.
                    if (y > y2)
                    {
                        std::swap(y, y2);
                    }
                    // Draw it.
                    for(int i = y; i <= y2; i++)
                    {
                        data[i * trueWidth + x] = blend(color, data[i * trueWidth + x]);
                    }
                    return;
                }
                // Time for some Bresenham! (Thanks Kildorf, and Mr. Jack E. Bresenham)
                int cx, cy, dx, dy;
                int xaccum, yaccum, xincre, yincre, xreset, yreset, xchange, ychange;
                bool done = false;

                dx = abs(x2 - x);
                dy = abs(y2 - y);
                cx = x;
                cy = y;
                xchange = (x > x2) ? -1 : 1;
                ychange = (y > y2) ? -1 : 1;

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

                    data[cy * trueWidth + cx] = blend(color, data[cy * trueWidth + cx]);

                    if(xreset == 0 && cx == x2) done = true;
                    if(yreset == 0 && cy == y2) done = true;
                }
            }

            template <typename BlendCallback> void rect(int x, int y, int x2, int y2, Color color, const BlendCallback& blend)
            {
                if(!data) return;
                int i;

                // Put the coordinates in order.
                if (x > x2)
                {
                    std::swap(x, x2);
                }
                if (y > y2)
                {
                    std::swap(y, y2);
                }
                // Don't draw if completely outside clipping regions.
                if(x > clipX2 || y > clipY2 || x2 < clipX || y2 < clipY)
                {
                    return;
                }
                // Keep rectangle inside clipping regions
                if(x < clipX)
                {
                    x = clipX;
                }
                if(x2 > clipX2)
                {
                    x2 = clipX2;
                }
                if(y < clipY)
                {
                    y = clipY;
                }
                if(y2 > clipY2)
                {
                    y2 = clipY2;
                }
                // Draw the horizontal lines of the rectangle.
                for(i = x; i <= x2; i++)
                {
                    data[y * trueWidth + i] = blend(color, data[y * trueWidth + i]);
                    data[y2 * trueWidth + i] = blend(color, data[y2 * trueWidth + i]);
                }
                // Draw the vertical lines of the rectangle.
                for(i = y; i <= y2; i++)
                {
                    data[i * trueWidth + x] = blend(color, data[i * trueWidth + x]);
                    data[i * trueWidth + x2] = blend(color, data[i * trueWidth + x2]);
                }
            }

            template <typename BlendCallback> void solidRect(int x, int y, int x2, int y2, Color color, const BlendCallback& blend)
            {
                if(!data) return;
                int i, j;

                if (x > x2)
                {
                    std::swap(x, x2);
                }
                if (y > y2)
                {
                    std::swap(y, y2);
                }
                // Don't draw if completely outside clipping regions.
                if(x > clipX2 || y > clipY2 || x2 < clipX || y2 < clipY)
                {
                    return;
                }
                // Keep rectangle inside clipping regions
                if(x < clipX)
                {
                    x = clipX;
                }
                if(x2 > clipX2)
                {
                    x2 = clipX2;
                }
                if(y < clipY)
                {
                    y = clipY;
                }
                if(y2 > clipY2)
                {
                    y2 = clipY2;
                }
                // Draw the solid rectangle
                for(i = y; i <= y2; i++)
                {
                    for(j = x; j <= x2; j++)
                    {
                        data[i * trueWidth + j] = blend(color, data[i * trueWidth + j]);
                    }
                }
            }

            template <typename BlendCallback> void horizontalGradientRect(int x, int y, int x2, int y2, Color color, Color color2, const BlendCallback& blend);
            template <typename BlendCallback> void verticalGradientRect(int x, int y, int x2, int y2, Color color, Color color2, const BlendCallback& blend);

            // Algorithm based off this paper:
            // "A Fast Bresenham Type Algorithm For Drawing Ellipses"
            // by John Kennedy
            // rkennedy@ix.netcom.com
            template <typename BlendCallback> void circle(int cx, int cy, int xRadius, int yRadius, Color color, const BlendCallback& blend)
            {
                if(!data) return;
                int x, y, plotX, plotY;
                int xChange, yChange;
                int ellipseError;
                int a, b;
                int stoppingX, stoppingY;
                int lastX = -1;
                int lastY = -1;
                a = 2 * xRadius * xRadius;
                b = 2 * yRadius * yRadius;
                x = xRadius;
                y = 0;
                xChange = yRadius * yRadius * (1 - 2 * xRadius);
                yChange = xRadius * xRadius;
                ellipseError = 0;
                stoppingX = b * xRadius;
                stoppingY = 0;

                // First set of points, y' > -1
                while(stoppingX >= stoppingY)
                {
                    if(lastX != x || lastY != y)
                    {
                        plotY = cy - y;
                        if(plotY >= clipY && plotY <= clipY2)
                        {
                            plotX = cx - x;
                            if(plotX >= clipX && plotX <= clipX2)
                            {
                                data[plotY * trueWidth + plotX] = blend(color, data[plotY * trueWidth + plotX]);
                            }
                            plotX = cx + x;
                            if(plotX >= clipX && plotX <= clipX2)
                            {
                                data[plotY * trueWidth + plotX] = blend(color, data[plotY * trueWidth + plotX]);
                            }
                        }
                        if(y)
                        {
                            plotY = cy + y;
                            if(plotY >= clipY && plotY <= clipY2)
                            {
                                plotX = cx - x;
                                if(plotX >= clipX && plotX <= clipX2)
                                {
                                    data[plotY * trueWidth + plotX] = blend(color, data[plotY * trueWidth + plotX]);
                                }
                                plotX = cx + x;
                                if(plotX >= clipX && plotX <= clipX2)
                                {
                                    data[plotY * trueWidth + plotX] = blend(color, data[plotY * trueWidth + plotX]);
                                }
                            }
                        }
                        lastX = x;
                        lastY = y;
                    }

                    y++;
                    stoppingY += a;
                    ellipseError += yChange;
                    yChange += a;
                    if(ellipseError * 2 + xChange > 0)
                    {
                        x--;
                        stoppingX -= b;
                        ellipseError += xChange;
                        xChange += b;
                    }
                }
                // First point set is done; start the second set of points
                x = 0;
                y = yRadius;
                xChange = yRadius * yRadius;
                yChange = xRadius * xRadius * (1 - yRadius * 2);
                ellipseError = 0;
                stoppingX = 0;
                stoppingY = a * yRadius;
                // Second set of points, y' < -1
                while(stoppingX <= stoppingY)
                {
                    if(lastX != x || lastY != y)
                    {
                        plotY = cy - y;
                        if(plotY >= clipY && plotY <= clipY2)
                        {
                            plotX = cx - x;
                            if(plotX >= clipX && plotX <= clipX2)
                            {
                                data[plotY * trueWidth + plotX] = blend(color, data[plotY * trueWidth + plotX]);
                            }
                            if(x)
                            {
                                plotX = cx + x;
                                if(plotX >= clipX && plotX <= clipX2)
                                {
                                    data[plotY * trueWidth + plotX] = blend(color, data[plotY * trueWidth + plotX]);
                                }
                            }
                        }
                        if(y)
                        {
                            plotY = cy + y;
                            if(plotY >= clipY && plotY <= clipY2)
                            {
                                plotX = cx - x;
                                if(plotX >= clipX && plotX <= clipX2)
                                {
                                    data[plotY * trueWidth + plotX] = blend(color, data[plotY * trueWidth + plotX]);
                                }
                                if(x)
                                {
                                    plotX = cx + x;
                                    if(plotX >= clipX && plotX <= clipX2)
                                    {
                                        data[plotY * trueWidth + plotX] = blend(color, data[plotY * trueWidth + plotX]);
                                    }
                                }
                            }
                        }
                        lastX = x;
                        lastY = y;
                    }

                    x++;
                    ellipseError += xChange;
                    xChange += b;
                    stoppingX += b;
                    if(ellipseError * 2 + yChange > 0)
                    {
                        y--;
                        ellipseError += yChange;
                        yChange += a;
                        stoppingY -= a;
                    }
                }
            }

            // Algorithm based off this paper:
            // "A Fast Bresenham Type Algorithm For Drawing Ellipses"
            // by John Kennedy
            // rkennedy@ix.netcom.com
            template <typename BlendCallback> void solidCircle(int cx, int cy, int xRadius, int yRadius, Color color, const BlendCallback& blend)
            {
                if(!data) return;
                int i, plotX, plotX2, plotY;
                int x, y;
                int xChange, yChange;
                int ellipseError;
                int a, b;
                int stoppingX, stoppingY;
                int lastY = -1;
                a = 2 * xRadius * xRadius;
                b = 2 * yRadius * yRadius;
                x = xRadius;
                y = 0;
                xChange = yRadius * yRadius * (1 - 2 * xRadius);
                yChange = xRadius * xRadius;
                ellipseError = 0;
                stoppingX = b * xRadius;
                stoppingY = 0;

                // First set of points, y' > -1
                while(stoppingX >= stoppingY)
                {
                    if(lastY != y)
                    {
                        plotX = std::max(cx - x, clipX);
                        plotX2 = std::min(cx + x, clipX2);
                        plotY = cy - y;
                        if(plotY >= clipY && plotY <= clipY2)
                        {
                            for(i = plotX; i <= plotX2; i++)
                            {
                                data[plotY * trueWidth + i] = blend(color, data[plotY * trueWidth + i]);
                            }
                        }
                        if(y)
                        {
                            plotY = cy + y;
                            if(plotY >= clipY && plotY <= clipY2)
                            {
                                for(i = plotX; i <= plotX2; i++)
                                {
                                    data[plotY * trueWidth + i] = blend(color, data[plotY * trueWidth + i]);
                                }
                            }
                            lastY = y;
                        }
                    }

                    y++;
                    stoppingY += a;
                    ellipseError += yChange;
                    yChange += a;
                    if(ellipseError * 2 + xChange > 0)
                    {
                        x--;
                        stoppingX -= b;
                        ellipseError += xChange;
                        xChange += b;
                    }
                }
                // First point set is done; start the second set of points
                x = 0;
                y = yRadius;
                xChange = yRadius * yRadius;
                yChange = xRadius * xRadius * (1 - yRadius * 2);
                ellipseError = 0;
                stoppingX = 0;
                stoppingY = a * yRadius;
                // Second set of points, y' < -1
                while(stoppingX <= stoppingY)
                {
                    if(lastY != y)
                    {
                        plotX = std::max(cx - x, clipX);
                        plotX2 = std::min(cx + x, clipX2);
                        plotY = cy - y;
                        if(plotY >= clipY && plotY <= clipY2)
                        {
                            for(i = plotX; i <= plotX2; i++)
                            {
                                data[plotY * trueWidth + i] = blend(color, data[plotY * trueWidth + i]);
                            }
                        }
                        plotY = cy + y;
                        if(plotY >= clipY && plotY <= clipY2)
                        {
                            for(i = plotX; i <= plotX2; i++)
                            {
                                data[plotY * trueWidth + i] = blend(color, data[plotY * trueWidth + i]);
                            }
                        }
                        lastY = y;
                    }

                    x++;
                    ellipseError += xChange;
                    xChange += b;
                    stoppingX += b;
                    if(ellipseError * 2 + yChange > 0)
                    {
                        y--;
                        ellipseError += yChange;
                        yChange += a;
                        stoppingY -= a;
                    }
                }
            }

            template <typename BlendCallback> void blit(int x, int y, Canvas& dest, const BlendCallback& blend) const
            {
                if(!data) return;
                int i, j;
                int x2 = x + trueWidth - 1;
                int y2 = y + trueHeight -1;
                int sourceX = 0;
                int sourceY = 0;
                int sourceX2 = trueWidth - 1;
                int sourceY2 = trueHeight - 1;

                // Don't draw if completely outside clipping regions.
                if(x > dest.clipX2 || y > dest.clipY2 || x2 < dest.clipX || y2 < dest.clipY)
                {
                    return;
                }
                // Keep rectangle inside clipping regions
                if(x < dest.clipX)
                {
                    sourceX += dest.clipX - x;
                }
                if(x2 > dest.clipX2)
                {
                    sourceX2 -= x2 - dest.clipX2;
                }
                if(y < dest.clipY)
                {
                    sourceY += dest.clipY - y;
                }
                if(y2 > dest.clipY2)
                {
                    sourceY2 -= y2 - dest.clipY2;
                }
                // Draw the image, pixel for pixel
                for(i = sourceY; i <= sourceY2; i++)
                {
                    for(j = sourceX; j <= sourceX2; j++)
                    {
                        dest.data[(i + y) * dest.trueWidth + (j + x)] = blend(data[i * trueWidth + j], dest.data[(i + y) * dest.trueWidth + (j + x)]);
                    }
                }
            }

            template <typename BlendCallback> void scaleBlit(int x, int y, int scaledWidth, int scaledHeight, Canvas& dest, const BlendCallback& blend) const
            {
                if(!data) return;
                scaleBlitRegion(0, 0, width, height, x, y, scaledWidth, scaledHeight, dest, blend);
            }

            template <typename BlendCallback> void rotateBlit(int x, int y, double angle, Canvas& dest, const BlendCallback& blend) const
            {
                if(!data) return;
                rotateScaleBlitRegion(0, 0, width, height, x, y, angle, 1, dest, blend);
            }

            template <typename BlendCallback> void rotateScaleBlit(int x, int y, double angle, double scale, Canvas& dest, const BlendCallback& blend) const
            {
                if(!data) return;
                rotateScaleBlitRegion(0, 0, width, height, x, y, angle, scale, dest, blend);
            }

            template <typename BlendCallback> void blitRegion(int sx, int sy, int sx2, int sy2,
                    int dx, int dy, Canvas& dest, const BlendCallback& blend) const
            {
                if(!data) return;
                int cx = dest.clipX;
                int cy = dest.clipY;
                int cx2 = dest.clipX2;
                int cy2 = dest.clipY2;

                if (sx > sx2)
                {
                    std::swap(sx, sx2);
                }
                if (sy > sy2)
                {
                    std::swap(sy, sy2);
                }

                dest.setClipRegion(dx, dy, dx + (sx2 - sx), dy + (sy2 - sy));
                blit(dx - sx, dy - sy, dest, blend);
                dest.setClipRegion(cx, cy, cx2, cy2);
            }

            template <typename BlendCallback> void scaleBlitRegion(int sx, int sy, int sx2, int sy2,
                    int dx, int dy, int scw, int sch, Canvas& dest, const BlendCallback& blend) const
            {
                if(!data) return;
                if (sx > sx2)
                {
                    std::swap(sx, sx2);
                }
                if (sy > sy2)
                {
                    std::swap(sy, sy2);
                }
                sx = std::min(std::max(0, sx), width - 1);
                sy = std::min(std::max(0, sy), height - 1);
                sx2 = std::min(std::max(0, sx2), width - 1);
                sy2 = std::min(std::max(0, sy2), height - 1);

                int i, j;
                int dx2 = dx + scw - 1;
                int dy2 = dy + sch - 1;
                int sourceX = 0;
                int sourceY = 0;
                int sourceX2 = scw - 1;
                int sourceY2 = sch - 1;
                int xRatio = ((sx2 - sx + 1) << 16) / scw;
                int yRatio = ((sy2 - sy + 1) << 16) / sch;

                // Don't draw if completely outside clipping regions.
                if(dx > dest.clipX2 || dy > dest.clipY2 || dx2 < dest.clipX || dy2 < dest.clipY)
                {
                    return;
                }
                // Keep rectangle inside clipping regions
                if(dx < dest.clipX)
                {
                    sourceX += dest.clipX - dx;
                }
                if(dx2 > dest.clipX2)
                {
                    sourceX2 -= dx2 - dest.clipX2;
                }
                if(dy < dest.clipY)
                {
                    sourceY += dest.clipY - dy;
                }
                if(dy2 > dest.clipY2)
                {
                    sourceY2 -= dy2 - dest.clipY2;
                }
                // Draw the scaled image, pixel for pixel
                for(i = sourceY; i <= sourceY2; i++)
                {
                    for(j = sourceX; j <= sourceX2; j++)
                    {
                        dest.data[(i + dy) * dest.trueWidth + (j + dx)] = blend(data[(((i * yRatio + sy) >> 16) + sy) * trueWidth + ((j * xRatio + sx) >> 16) + sx], dest.data[(i + dy) * dest.trueWidth + (j + dx)]);
                    }
                }        
            }

            template <typename BlendCallback> void rotateBlitRegion(int sx, int sy, int sx2, int sy2,
                    int dx, int dy, double angle, Canvas& dest, const BlendCallback& blend) const
            {
                if(!data) return;
                rotateScaleBlitRegion(sx, sy, sx2, sy2, dx, dy, angle, 1.0, dest, blend);
            }

            template <typename BlendCallback> void rotateScaleBlitRegion(int sx, int sy, int sx2, int sy2,
                    int dx, int dy, double angle, double scale, Canvas& dest, const BlendCallback& blend) const
            {
                if(!data) return;
                int minX, minY;
                int maxX, maxY;
                int destX, destY;
                int centerX, centerY;
                int plotX, plotY;
                int sourceX, sourceY;
                int cosine, sine;
                int cosCenterX, sinCenterX;
                int cosCenterY, sinCenterY;
                int a;

                // I like angles in degrees better when calling this.
                // So now let's convert this to work all nice-like with C++'s math which is radians.
                angle *= M_PI / 180;

                if (sx > sx2)
                {
                    std::swap(sx, sx2);
                }
                if (sy > sy2)
                {
                    std::swap(sy, sy2);
                }
                sx = std::min(std::max(0, sx), width - 1);
                sy = std::min(std::max(0, sy), height - 1);
                sx2 = std::min(std::max(0, sx2), width - 1);
                sy2 = std::min(std::max(0, sy2), height - 1);

                cosine = int(cos(angle) * 65536);
                sine = int(sin(angle) * 65536);

                centerX = int((sx2 - sx) * scale / 2);
                centerY = int((sy2 - sy) * scale / 2);

                sinCenterX = centerX * sine;
                sinCenterY = centerY * sine;
                cosCenterX = centerX * cosine;
                cosCenterY = centerY * cosine;

                // The minimum (x, y) are the smallest of all rotated endpoints (x, y), or destination's (clipX, clipY), whichever are greater.
                // The maximum (x, y) are the largest of all rotated endpoints (x, y), or destination's (clipX2, clipY2), whichever are smaller.
                // The idea is to get the smallest box area that contains the rotated image (inside the clipping region)
                // Note: Hand-optimized because 4-deep macros meant a lot of repeated calculations,
                // that compile optimization doesn't seem capable of getting rid of.
                minX = (cosCenterX + sinCenterY) >> 16;
                maxX = minX;
                // Since cosCenterX - sinCenterY == -(cosCenterX + sinCenterY), save calculations.
                a = -minX;
                minX = std::min(minX, a);
                maxX = std::max(maxX, a);
                a = (cosCenterX - sinCenterY) >> 16;
                minX = std::min(minX, a);
                maxX = std::max(maxX, a);
                // Since -cosCenterX + sinCenterY == -(cosCenterX - sinCenterY), save calculations.
                a = -a;
                minX = std::min(minX, a);
                maxX = std::max(maxX, a);
                // Finally, clipping
                minX = std::max(minX + dx, dest.clipX);
                maxX = std::min(maxX + dx, dest.clipX2);

                minY = (cosCenterY + sinCenterX) >> 16;
                maxY = minY;
                // Since cosCenterY - sinCenterX == -(cosCenterY + sinCenterX), save calculations.
                a = -minY;
                minY = std::min(minY, a);
                maxY = std::max(maxY, a);
                a = (cosCenterX - sinCenterY) >> 16;
                minY = std::min(minY, a);
                maxY = std::max(maxY, a);
                // Since -cosCenterY + sinCenterX == -(cosCenterY - sinCenterX), save calculations.
                a = -a;
                minY = std::min(minY, a);
                maxY = std::max(maxY, a);
                // Finally, clipping
                minY = std::max(minY + dy, dest.clipY);
                maxY = std::min(maxY + dy, dest.clipY2);

                centerX = ((sx2 - sx) << 15) + (sx << 16); 
                centerY = ((sy2 - sy) << 15) + (sy << 16);

                sine = int(sine / scale);
                cosine = int(cosine / scale);

                for (destY = minY; destY < maxY; destY++)
                {
                    plotX = (minX - dx) * cosine + (destY - dy) * sine + centerX;
                    plotY = (destY - dy) * cosine - (minX - dx) * sine + centerY;
                    for (destX = minX; destX < maxX; destX++)
                    {
                        sourceX = plotX >> 16;
                        sourceY = plotY >> 16;
                        if(sourceX >= sx && sourceX <= sx2 && sourceY >= sy && sourceY <= sy2)
                        {
                            dest.data[destY * dest.trueWidth + destX] = blend(data[sourceY * trueWidth + sourceX], dest.data[destY * dest.trueWidth + destX]);
                        }
                        plotX += cosine;
                        plotY -= sine;
                    }
                }
            }

        private:
            int width, height;
            int trueWidth, trueHeight;

            int clipX, clipY;
            int clipX2, clipY2;

            Color* data;
    };
}

#endif