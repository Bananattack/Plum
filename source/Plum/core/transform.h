#pragma once
#include "color.h"
#include "blending.h"

namespace plum
{
    struct Point
    {
        double x, y;
        Point()
        {
        }

        Point(double x, double y)
            : x(x), y(y)
        {
        }
    };

    struct Rect
    {
        double x, y;
        double width, height;

        Rect()
        {
        }

        Rect(double x, double y, double width, double height)
            : x(x), y(y), width(width), height(height)
        {
        }
    };

    // Associates a bunch of transformations together in one structure.
    // Makes it nicer to lower argument count and coupling in functions.
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

        // The position to translate the object to. Required.
        Point* position;
        // Rotation pivot point relative to top-left image corner. Required.
        Point* pivot;
        // Scale proportions. Required.
        Point* scale;

        // Clipping rectangle. NULL = whole image.
        Rect* clip;
        
        Transform()
        {
            mirror = false;
            angle = 0;
            mode = BlendAlpha;
            tint = Color::White;

            position = NULL;
            pivot = NULL;
            scale = NULL;
            clip = NULL;
        }
    };
}