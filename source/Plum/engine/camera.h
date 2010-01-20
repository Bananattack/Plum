#pragma once

namespace Plum
{
	struct Camera
	{
		// The world coordinates of the camera.
		double x, y;

		// The viewport region on screen which the camera is displayed at.
		// x < x2, y < y2 (swap if needed)
		int destX, destY;
		int destX2, destY2;

		Camera()
		{
			x = y = 0.0;
			destX = destY = destX2 = destY2 = 0;
		}

		~Camera();
	};
}