#ifndef PLUM_COLOR_H
#define PLUM_COLOR_H

#include <cstdint>
#include <algorithm>

namespace plum
{
    int getOpacity();

    enum ColorChannel
    {
        RedChannel = 0,
        GreenChannel = 1,
        BlueChannel = 2,
        AlphaChannel = 3
    };

    class Color
    {
        public:
            // Colors assume a 32-bit RGBA format
            enum
            {
                White = 0xFFFFFFFF,
                Red = 0xFF0000FF,
                Green = 0xFF00FF00,
                Blue = 0xFFFF0000,
                Magenta = 0xFFFF00FF,
                Cyan = 0xFFFFFF00,
                Yellow = 0xFF00FFFF,
                Black = 0xFF000000
            };

            operator uint32_t() const
            {
                return value;
            }

            bool operator !=(const Color& rhs) const
            {
                return value != rhs.value;
            }

            bool operator ==(const Color& rhs) const
            {
                return value == rhs.value;
            }

            uint8_t operator[](ColorChannel i) const
            {
                return (value >> (i << 3)) & 0xFF;
            }

            Color()
                : value(0)
            {
                value = 0;
            }

            Color(uint32_t value)
                : value(value)
            {
            }

            Color(uint8_t r, uint8_t g, uint8_t b)
                : value(0xFF000000 | (b << 16) | (g << 8) | r)
            {
            }

            Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
                : value((a << 24) | (b << 16) | (g << 8) | r)
            {
            }

            void channels(uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a) const
            {
                r = (*this)[RedChannel];
                g = (*this)[GreenChannel];
                b = (*this)[BlueChannel];
                a = (*this)[AlphaChannel];
            }

        private:
            uint32_t value;
    };

    inline Color rgba(int r, int g, int b, int a)
    {
        return Color(std::min(std::max(0, r), 255), std::min(std::max(0, g), 255), std::min(std::max(0, b), 255), std::min(std::max(0, a), 255));
    }
    
    inline Color hsv(int h, int s, int v, int a = 255)
    // Pass: Hue (0-359), Saturation (0-255), Value (0-255)[, Alpha (0-255).]
    // Credit goes to Zip for original conversion code.
    // Returns: a color determined using HSV color mapping.
    {
        int r, g, b = 0;
        int ixz;
        int f;

        h = (h + 360) % 360;
        s = std::min(std::max(0, s), 255);
        v = std::min(std::max(0, v), 255);

        ixz = h / 60;
        h = (h << 8) / 60;
        f = h - (ixz << 8);
        h = (v * (255 - ((s * (255 - f)) >> 8))) >> 8;    // t =
        f = (v * (255 - ((s * f) >> 8))) >> 8; // q =
        s = (v * (255 - s)) >> 8; // p =

        switch(ixz)
        {
            case 0: r = v; g = h; b = s; break;
            case 1: r = f; g = v; b = s; break;
            case 2: r = s; g = v; b = h; break;
            case 3: r = s; g = f; b = v; break;
            case 4: r = h; g = s; b = v; break;
            case 5: r = v; g = s; b = f; break;
        }
        return Color(r, g, b, a);
    }
}

#endif