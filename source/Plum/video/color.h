#ifndef PLUM_COLOR_H
#define PLUM_COLOR_H

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
}
#endif
