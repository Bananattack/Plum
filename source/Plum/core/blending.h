#ifndef PLUM_BLENDING_H
#define PLUM_BLENDING_H

#include "color.h"

#include <algorithm>

namespace plum
{
    enum BlendMode
    {
        BlendOpaque,     // Direct copy to destination
        BlendMerge,      // Combines two images, altering alpha as needed.
        BlendPreserve,      // Combines two images, preserving alpha of the destination.
        BlendAdd,        // Adds the RGB value of the source to the dest
        BlendSubtract    // Subtracts the RGB value of the source from the dest
    };

    int getOpacity();
    void setOpacity(int alpha);

    struct SoftOpaqueBlender
    {
        void operator()(const Color& source, Color& dest) const
        {
            dest = source;
        }
    };

    struct SoftMergeBlender
    {
        void operator()(const Color& source, Color& dest) const
        {
            int sourceAlpha = source[AlphaChannel] * getOpacity() / 255;
            int finalAlpha = sourceAlpha + ((255 - sourceAlpha) * dest[AlphaChannel]) / 255;
            sourceAlpha = (finalAlpha == 0) ? 0 : sourceAlpha * 255 / finalAlpha;

            dest = Color(
                uint8_t((sourceAlpha * int(source[RedChannel]) + (255 - sourceAlpha) * int(dest[RedChannel])) / 255),
                uint8_t((sourceAlpha * int(source[GreenChannel]) + (255 - sourceAlpha) * int(dest[GreenChannel])) / 255),
                uint8_t((sourceAlpha * int(source[BlueChannel]) + (255 - sourceAlpha) * int(dest[BlueChannel])) / 255),
                finalAlpha);
        }
    };

    struct SoftPreserveBlender
    {
        void operator()(const Color& source, Color& dest) const
        {
            int sourceAlpha = source[AlphaChannel] * getOpacity() / 255;
            dest = Color(
                uint8_t((sourceAlpha * (int(source[RedChannel]) - int(dest[RedChannel]))) / 255 + int(dest[RedChannel])),
                uint8_t((sourceAlpha * (int(source[GreenChannel]) - int(dest[GreenChannel]))) / 255 + int(dest[GreenChannel])),
                uint8_t((sourceAlpha * (int(source[BlueChannel]) - int(dest[BlueChannel]))) / 255 + int(dest[BlueChannel])),
                dest[AlphaChannel]);
        }
    };

    struct SoftAddBlender
    {
        void operator()(const Color& source, Color& dest) const
        {
            int sourceAlpha = source[AlphaChannel] * getOpacity() / 255;
            dest = Color(
                uint8_t(std::min((sourceAlpha * int(source[RedChannel])) / 255 + int(dest[RedChannel]), 255)),
                uint8_t(std::min((sourceAlpha * int(source[GreenChannel])) / 255 + int(dest[GreenChannel]), 255)),
                uint8_t(std::min((sourceAlpha * int(source[BlueChannel])) / 255 + int(dest[BlueChannel]), 255)),
                dest[AlphaChannel]);
        }
    };

    struct SoftSubtractBlender
    {
        void operator()(const Color& source, Color& dest) const
        {
            int sourceAlpha = source[AlphaChannel] * getOpacity() / 255;
            dest = Color(
                uint8_t(std::max((sourceAlpha * -int(source[RedChannel])) / 255 + int(dest[RedChannel]), 0)),
                uint8_t(std::max((sourceAlpha * -int(source[GreenChannel])) / 255 + int(dest[GreenChannel]), 0)),
                uint8_t(std::max((sourceAlpha * -int(source[BlueChannel])) / 255 + int(dest[BlueChannel]), 0)),
                dest[AlphaChannel]);
        }
    };
}

#endif