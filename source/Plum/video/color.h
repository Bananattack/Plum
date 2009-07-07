#pragma once

namespace Plum
{
	int getOpacity();

	enum ColorChannels
	{
		RedChannel = 0,
		GreenChannel = 1,
		BlueChannel = 2,
		AlphaChannel = 3
	};
	typedef unsigned char ColorChannel;

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


			int value;

			inline operator int() const
			{
				return value;
			}

			inline bool operator != (Color& rhs)
			{
				return (this->value != rhs.value);
			}

			inline bool operator == (Color& rhs)
			{
				return (this->value == rhs.value);
			}

			inline ColorChannel& operator[](ColorChannels i)
			{
				return ((ColorChannel*) &(this->value))[i];
			}

			Color()
			{
				this->value = 0;
			}

			Color(unsigned int value)
			{
				this->value = value;
			}

			Color(int r, int g, int b)
			{
				(*this)[RedChannel] = r;
				(*this)[GreenChannel] = g;
				(*this)[BlueChannel] = b;
				(*this)[AlphaChannel] = 255;
			}

			Color(int r, int g, int b, int a)
			{
				(*this)[RedChannel] = r;
				(*this)[GreenChannel] = g;
				(*this)[BlueChannel] = b;
				(*this)[AlphaChannel] = a;
			}
	};

	inline Color rgb(int r, int g, int b, int a = 255)
	{
		return Color(r, g, b, a);
	}

	inline Color rgba(int r, int g, int b, int a)
	{
		return Color(r, g, b, a);
	}

	inline void getRGB(Color color, ColorChannel& r, ColorChannel& g, ColorChannel& b)
	{
		r = color[RedChannel];
		g = color[GreenChannel];
		b = color[BlueChannel];
	}

	inline void getRGBA(Color color, ColorChannel& r, ColorChannel& g, ColorChannel& b, ColorChannel& a)
	{
		r = color[RedChannel];
		g = color[GreenChannel];
		b = color[BlueChannel];
		a = color[AlphaChannel];
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
		if (s > 255) s = 255;
		else if (s < 0) s = 0;
		if (v > 255) v = 255;
		else if (v < 0) v = 0;

		ixz = h / 60;
		h = (h << 8) / 60;
		f = h - (ixz << 8);
		h = (v * (255 - ((s * (255 - f)) >> 8))) >> 8;	// t =
		f = (v * (255 - ((s * f) >> 8))) >> 8; // q =
		s = (v * (255 - s)) >> 8; // p =

		switch (ixz)
		{
			case 0:
				r = v; g = h; b = s;
				break;
			case 1:
				r = f; g = v; b = s;
				break;
			case 2:
				r = s; g = v; b = h;
				break;
			case 3:
				r = s; g = f; b = v;
				break;
			case 4:
				r = h; g = s; b = v;
				break;
			case 5:
				r = v; g = s; b = f;
				break;
		}
		return Color(r, g, b, a);
	}
}
