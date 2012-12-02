#pragma once

#include "color.h"

#include <algorithm>

namespace plum
{
    enum BlendMode
    {
        BlendOpaque,     // Direct copy to destination
        BlendMerge,      // Combines two images, altering alpha as needed.
        BlendAlpha,      // Combines two images, preserving alpha of the destination.
        BlendAdd,        // Adds the RGB value of the source to the dest
        BlendSubtract    // Subtracts the RGB value of the source from the dest
    };

    int getOpacity();
    void setOpacity(int alpha);

    struct SoftOpaqueBlender
    {
        Color operator() (Color source, Color dest) const
        {
            return source;
        }
    };

    struct SoftMergeBlender
    {
        Color operator() (Color source, Color dest) const
        {
            Color result;
            int sourceAlpha = source[AlphaChannel] * getOpacity() / 255;
            int finalAlpha = sourceAlpha + ((255 - sourceAlpha) * dest[AlphaChannel]) / 255;
            sourceAlpha = (finalAlpha == 0) ? 0 : sourceAlpha * 255 / finalAlpha;

            result[RedChannel] = (source[RedChannel] * sourceAlpha + dest[RedChannel] * (255 - sourceAlpha)) / 255;
            result[GreenChannel] = (source[GreenChannel] * sourceAlpha + dest[GreenChannel] * (255 - sourceAlpha)) / 255;
            result[BlueChannel] = (source[BlueChannel] * sourceAlpha + dest[BlueChannel] * (255 - sourceAlpha)) / 255;
            result[AlphaChannel] = finalAlpha;

            return result;
        }
    };

    struct SoftPreserveBlender
    {
        Color operator() (Color source, Color dest) const
        {
            Color result;
            int sourceAlpha = source[AlphaChannel] * getOpacity() / 255;

            result[RedChannel] = (sourceAlpha * (source[RedChannel] - dest[RedChannel])) / 255 + dest[RedChannel];
            result[GreenChannel] = (sourceAlpha * (source[GreenChannel] - dest[GreenChannel])) / 255 + dest[GreenChannel];
            result[BlueChannel] = (sourceAlpha * (source[BlueChannel] - dest[GreenChannel])) / 255 + dest[BlueChannel];
            result[AlphaChannel] = dest[AlphaChannel];
            return result;
        }
    };

    struct SoftAddBlender
    {
        Color operator() (Color source, Color dest) const
        {
            Color result;
            int sourceAlpha = source[AlphaChannel] * getOpacity() / 255;

            result[RedChannel] = std::min((sourceAlpha * source[RedChannel]) / 255 + dest[RedChannel], 255);
            result[GreenChannel] = std::min((sourceAlpha * source[GreenChannel]) / 255 + dest[GreenChannel], 255);
            result[BlueChannel] = std::min((sourceAlpha * source[BlueChannel]) / 255 + dest[BlueChannel], 255);
            result[AlphaChannel] = dest[AlphaChannel];
            return result;
        }
    };

    struct SoftSubtractBlender
    {
        Color operator() (Color source, Color dest) const
        {
            Color result;
            int sourceAlpha = source[AlphaChannel] * getOpacity() / 255;

            result[RedChannel] = std::max((sourceAlpha * -source[RedChannel]) / 255 + dest[RedChannel], 0);
            result[GreenChannel] = std::max((sourceAlpha * -source[GreenChannel]) / 255 + dest[GreenChannel], 0);
            result[BlueChannel] = std::max((sourceAlpha * -source[BlueChannel]) / 255 + dest[BlueChannel], 0);
            result[AlphaChannel] = dest[AlphaChannel];
            return result;
        }
    };
}
