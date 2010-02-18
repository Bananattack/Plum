#pragma once
namespace Plum
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
		double x, y, x2, y2;

		Rect()
		{
		}

		Rect(double x, double y, double x2, double y2)
			: x(x), y(y), x2(x2), y2(y2)
		{
		}
	};

	// Associates a bunch of transformations together in one structure.
	// Makes it nicer to lower argument count and coupling in functions.
	struct Transform
	{
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
			angle = 0;
			mode = BlendUnspecified;
			tint = Color::White;

			position = NULL;
			pivot = NULL;
			scale = NULL;
			clip = NULL;
		}
	};
}