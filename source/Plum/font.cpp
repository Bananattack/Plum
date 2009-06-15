#include "plum.h"

namespace Plum
{
	void Font::init(const char* filename)
	{
		texture = new Texture(filename);
		letterSpacing = 2;

		Image* image = texture->getImage();

		// Try to automatically detect the font size based on the border edges.
		int w = 0;
		int h = 0;
		Color c = image->getPixel(0, 0);
		for (w = 1; w < image->width; w++)
		{
			Color z = image->getPixel(w, 1);
			if (z == c)
				break;
		}
		for (h = 1; h < image->height; h++)
		{
			Color z = image->getPixel(1, h);
			if (z == c)
				break;
		}
		int xsize = width = w - 1;
		int ysize = height = h - 1;

		// Initialize glyph widths, which might be replaced later if variable width mode is enabled.
		for(int i = 0; i < FONT_COLUMNS * FONT_ROWS; i++)
		{
			glyphWidth[i] = width;
		}
	}

	bool Font::isColumnEmpty(int cell, int column)
	{
		int fx = (cell % FONT_COLUMNS) * (width + 1) + 1;
		int fy = (cell / FONT_COLUMNS) * (height + 1) + 1;
		Image* image = texture->getImage();
		for(int y = 0; y < height; y++)
		{
			if(image->getPixel(fx + column, fy + y)[AlphaChannel] > 0)
			{
				return false;
			}
		}
		return true;
	}

	void Font::enableVariableWidth()
	{
		glyphWidth[0] = width * 60 / 100;
		for (int i = 1; i < FONT_COLUMNS * FONT_ROWS; i++)
		{
			glyphWidth[i] = 1;
			for (int x = width - 1; x > 0; x--)
			{
				if (!isColumnEmpty(i, x))
				{
					glyphWidth[i] = x + 1;
					break;
				}
			}
			if (glyphWidth[i] == 1)
			{
				glyphWidth[i] = glyphWidth[0];
			}
		}
	}

	void Font::printChar(int x, int y, char c, BlendMode mode)
	{
		// Only allow printable characters.
		// Don't need to check for >= 128 because that would make c negative.
		// (this uses ASCII and will possibly need to be replaced with something much more clever some time down the line.)
		if (c < 32) return;

		int fx = ((c - 32) % FONT_COLUMNS) * (width + 1) + 1;
		int fy = ((c - 32) / FONT_COLUMNS) * (height + 1) + 1;

		texture->blitRegion(fx, fy, fx + width - 1, fy + height - 1, x, y, mode, Color::White);
	}

	void Font::print(int x, int y, std::string s, BlendMode mode)
	{
		mode = (mode == BlendUnspecified) ? getBlendMode() : mode;
		for (unsigned int i = 0; i < s.length(); i++)
		{
			if (s[i] < 32)
			{
				continue;
			}
			printChar(x, y, s[i], mode);
			x += glyphWidth[s[i]] + letterSpacing;
		}
	}
}