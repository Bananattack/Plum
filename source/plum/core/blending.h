#ifndef PLUM_BLENDING_H
#define PLUM_BLENDING_H

#include "color.h"

#include <algorithm>

namespace plum
{
    enum class BlendMode
    {
        Opaque,     // Direct copy to destination
        Merge,      // Combines two images, altering alpha as needed.
        Preserve,      // Combines two images, preserving alpha of the destination.
        Add,        // Adds the RGB value of the source to the dest
        Subtract    // Subtracts the RGB value of the source from the dest
    };

    template<BlendMode Blend> void blend(const Color& source, Color& dest, int opacity);

    template<> inline void blend<BlendMode::Opaque>(const Color& source, Color& dest, int opacity)
    {
        dest = source;
    }

    template<> inline void blend<BlendMode::Merge>(const Color& source, Color& dest, int opacity)
    {
        int sourceAlpha = source[ColorChannel::Alpha] * opacity / 255;
        int finalAlpha = sourceAlpha + ((255 - sourceAlpha) * dest[ColorChannel::Alpha]) / 255;
        sourceAlpha = (finalAlpha == 0) ? 0 : sourceAlpha * 255 / finalAlpha;

        dest = Color(
            uint8_t((sourceAlpha * int(source[ColorChannel::Red]) + (255 - sourceAlpha) * int(dest[ColorChannel::Red])) / 255),
            uint8_t((sourceAlpha * int(source[ColorChannel::Green]) + (255 - sourceAlpha) * int(dest[ColorChannel::Green])) / 255),
            uint8_t((sourceAlpha * int(source[ColorChannel::Blue]) + (255 - sourceAlpha) * int(dest[ColorChannel::Blue])) / 255),
            finalAlpha);
    }

    template<> inline void blend<BlendMode::Preserve>(const Color& source, Color& dest, int opacity)
    {
        int sourceAlpha = source[ColorChannel::Alpha] * opacity / 255;
        dest = Color(
            uint8_t((sourceAlpha * (int(source[ColorChannel::Red]) - int(dest[ColorChannel::Red]))) / 255 + int(dest[ColorChannel::Red])),
            uint8_t((sourceAlpha * (int(source[ColorChannel::Green]) - int(dest[ColorChannel::Green]))) / 255 + int(dest[ColorChannel::Green])),
            uint8_t((sourceAlpha * (int(source[ColorChannel::Blue]) - int(dest[ColorChannel::Blue]))) / 255 + int(dest[ColorChannel::Blue])),
            dest[ColorChannel::Alpha]);
    }

    template<> inline void blend<BlendMode::Add>(const Color& source, Color& dest, int opacity)
    {
        int sourceAlpha = source[ColorChannel::Alpha] * opacity / 255;
        dest = Color(
            uint8_t(std::min((sourceAlpha * int(source[ColorChannel::Red])) / 255 + int(dest[ColorChannel::Red]), 255)),
            uint8_t(std::min((sourceAlpha * int(source[ColorChannel::Green])) / 255 + int(dest[ColorChannel::Green]), 255)),
            uint8_t(std::min((sourceAlpha * int(source[ColorChannel::Blue])) / 255 + int(dest[ColorChannel::Blue]), 255)),
            dest[ColorChannel::Alpha]);
    }

    template<> inline void blend<BlendMode::Subtract>(const Color& source, Color& dest, int opacity)
    {
        int sourceAlpha = source[ColorChannel::Alpha] * opacity / 255;
        dest = Color(
            uint8_t(std::max((sourceAlpha * -int(source[ColorChannel::Red])) / 255 + int(dest[ColorChannel::Red]), 0)),
            uint8_t(std::max((sourceAlpha * -int(source[ColorChannel::Green])) / 255 + int(dest[ColorChannel::Green]), 0)),
            uint8_t(std::max((sourceAlpha * -int(source[ColorChannel::Blue])) / 255 + int(dest[ColorChannel::Blue]), 0)),
            dest[ColorChannel::Alpha]);
    }
}

#endif