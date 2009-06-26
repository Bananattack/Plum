#include "../plum.h"

namespace Plum
{
	void Font::init(const char* filename)
	{
		texture = new Texture(filename);
		letterSpacing = 1;

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

	void Font::printChar(int x, int y, char c, BlendMode mode, Color tint)
	{
		// Only allow printable characters.
		// Don't need to check for >= 128 because that would make c negative.
		// (this uses ASCII and will possibly need to be replaced with something much more clever some time down the line.)
		if (c < 32) return;

		int fx = ((c - 32) % FONT_COLUMNS) * (width + 1) + 1;
		int fy = ((c - 32) / FONT_COLUMNS) * (height + 1) + 1;

		texture->blitRegion(fx, fy, fx + width - 1, fy + height - 1, x, y, mode, tint);
	}

	void Font::print(int x1, int y, std::string s, BlendMode mode, Color tint)
	{
		mode = (mode == BlendUnspecified) ? getBlendMode() : mode;
		int x = x1;
		for (unsigned int i = 0; i < s.length(); i++)
		{
			if(s[i] == '\n')
			{
				x = x1;
				y += height;
			}
			else if(s[i] == '\t')
			{
				x += glyphWidth[0] * 4;
			}
			else if (s[i] >= 32)
			{
				printChar(x, y, s[i], mode, tint);
				x += glyphWidth[s[i] - 32] + letterSpacing;
			}
		}
	}

	void Font::printCenter(int x1, int y, std::string s, BlendMode mode, Color tint)
	{
		mode = (mode == BlendUnspecified) ? getBlendMode() : mode;
		int x = x1;
		int lineIndex = 0;
		int ofs = lineWidth(s, 0) / 2;
		for (unsigned int i = 0; i < s.length(); i++)
		{
			if(s[i] == '\n')
			{
				x = x1;
				y += height;

				lineIndex++;
				ofs = lineWidth(s, lineIndex) / 2;
			}
			else if(s[i] == '\t')
			{
				x += glyphWidth[0] * 4;
			}
			else if (s[i] >= 32)
			{
				printChar(x - ofs, y, s[i], mode, tint);
				x += glyphWidth[s[i] - 32] + letterSpacing;
			}
		}
	}

	void Font::printRight(int x1, int y, std::string s, BlendMode mode, Color tint)
	{
		mode = (mode == BlendUnspecified) ? getBlendMode() : mode;
		int x = x1;
		int lineIndex = 0;
		int ofs = lineWidth(s, 0);
		for (unsigned int i = 0; i < s.length(); i++)
		{
			if(s[i] == '\n')
			{
				x = x1;
				y += height;

				lineIndex++;
				ofs = lineWidth(s, lineIndex);
			}
			else if(s[i] == '\t')
			{
				x += glyphWidth[0] * 4;
			}
			else if (s[i] >= 32)
			{
				printChar(x - ofs, y, s[i], mode, tint);
				x += glyphWidth[s[i] - 32] + letterSpacing;
			}
		}
	}

	int Font::lineWidth(std::string s, int lineIndex)
	{
		unsigned int i;
		int w = 0;
		int lineOffset = 0;
		for (i = 0; i < s.length(); i++)
		{
			char c = s[i];
			
			if (c == '\n')
			{
				if (lineOffset == lineIndex)
				{
					return w;
				}
				lineOffset++;
				w = 0;
			}
			else if (c == '\t')
			{
				w += glyphWidth[0] * 4;
			}
			else if (c >= 32)
			{
				w += glyphWidth[s[i] - 32] + letterSpacing;
			}
		}
		return w;
	}

	int Font::lineCount(std::string s)
	{
		int c = 1;
		for (unsigned int i = 0; i < s.length(); i++)
		{
			if(s[i] == '\n')
			{
				c++;
			}
		}
		return c;
	}

	int Font::textWidth(std::string s)
	{
		int c = lineCount(s);
		int w = 0;
		for(int i = 0; i < c; i++)
		{
			w = PLUM_MAX(w, lineWidth(s, i));
		}
		return w;
	}

	int Font::textHeight(std::string s)
	{
		return lineCount(s) * height;
	}
}