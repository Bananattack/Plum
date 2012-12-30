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

    inline Color rgb(int red, int green, int blue, int alpha = 255)
    {
        return Color(
            std::min(std::max(0, red), 255),
            std::min(std::max(0, green), 255),
            std::min(std::max(0, blue), 255),
            std::min(std::max(0, alpha), 255));
    }
    
    // Credit goes to Zip for original conversion code.
    inline Color hsv(int hue, int saturation, int value, int alpha = 255)
    {
        uint32_t h = (hue % 360 + 360) % 360;
        uint32_t s = std::min(std::max(0, saturation), 255);
        uint32_t v = std::min(std::max(0, value), 255);
        uint32_t a = std::min(std::max(0, alpha), 255);
        uint32_t i = h / 60;
        uint32_t f = (h << 8) / 60 - (i << 8);
        uint32_t p = (v * (255 - s)) >> 8;
        uint32_t q = (v * (255 - ((s * f) >> 8))) >> 8;
        uint32_t t = (v * (255 - ((s * (255 - f)) >> 8))) >> 8;
        switch(i)
        {
            case 0: return Color(v, t, p, a);
            case 1: return Color(q, v, p, a);
            case 2: return Color(p, v, t, a);
            case 3: return Color(p, q, v, a);
            case 4: return Color(t, p, v, a);
            case 5: return Color(v, p, q, a);
            default: return 0;
        }
    }
}

#endif