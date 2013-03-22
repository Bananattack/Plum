#ifndef PLUM_TRANSFORM_H
#define PLUM_TRANSFORM_H

#include "color.h"
#include "blending.h"

namespace plum
{
    struct Transform
    {
        // Flip horizontally.
        bool mirror;
        // Angle of rotation. Defaults to 0.
        double angle;
        // Blending mode. Defaults to Unspecified (inherit global).
        BlendMode mode;
        // Image tint. Defaults to White.
        Color tint;
        // Scale proportions.
        double scaleX, scaleY;
        
        Transform()
            : mirror(false),
            angle(0.0),
            mode(BlendPreserve),
            tint(Color::White),
            scaleX(1.0),
            scaleY(1.0)
        {
        }
    };
}

#endif