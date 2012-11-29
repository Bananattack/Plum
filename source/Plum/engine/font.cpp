#include "font.h"
#include "../video/canvas.h"
#include "../video/image.h"

namespace plum
{
    Font::Font(const std::string& filename)
        : image(new Image(filename)), letterSpacing(1)
    {
        Canvas* canvas = image->getCanvas();

        // Try to automatically detect the font size based on the border edges.
        int w = 0;
        int h = 0;
        Color c = canvas->getPixel(0, 0);
        for (w = 1; w < canvas->width; w++)
        {
            Color z = canvas->getPixel(w, 1);
            if (z == c)
                break;
        }
        for (h = 1; h < canvas->height; h++)
        {
            Color z = canvas->getPixel(1, h);
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

    Font::~Font()
    {
        delete image;
    }

    bool Font::isColumnEmpty(int cell, int column)
    {
        int fx = (cell % FONT_COLUMNS) * (width + 1) + 1;
        int fy = (cell / FONT_COLUMNS) * (height + 1) + 1;
        Canvas* canvas = image->getCanvas();
        for(int y = 0; y < height; y++)
        {
            if(canvas->getPixel(fx + column, fy + y)[AlphaChannel] > 0)
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

        image->blitRegion(fx, fy, fx + width - 1, fy + height - 1, x, y, mode, tint);
    }

    void Font::print(int x1, int y, const std::string& s, BlendMode mode, Color tint)
    {
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

    void Font::printRight(int x1, int y, const std::string& s, BlendMode mode, Color tint)
    {
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

    void Font::printCenter(int x1, int y, const std::string& s, BlendMode mode, Color tint)
    {
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

    int Font::lineWidth(const std::string& s, int lineIndex)
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

    int Font::lineCount(const std::string& s)
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

    int Font::textWidth(const std::string& s)
    {
        int c = lineCount(s);
        int w = 0;
        for(int i = 0; i < c; i++)
        {
            w = std::max(w, lineWidth(s, i));
        }
        return w;
    }

    int Font::textHeight(const std::string& s)
    {
        return lineCount(s) * height;
    }

    // Overkill: 2005-12-19
    // Thank you, Zip.
    // Rewritten, Kildorf: 2007-10-16
    // Rewritten, Overkill: 2010-02-18
    std::string Font::wrapText(const std::string& input, int lineLength)
    // Pass: The string to wrap, the length in pixels to fit into
    // Return: The passed string with \n characters inserted as breaks
    // Assmes: The font is valid, and will overrun if a word is longer than linelen
    // Note: Existing breaks will be respected, but adjacent \n characters will be
    //     replaced with a single \n so add a space for multiple line breaks
    {
        // Beginning of the current line
        int lastBreak = -1;
        // Last whitespace character
        int lastWhitespace = -1;
        // Current character
        int pos = 0;
        // Length of string
        int len = input.length();

        std::string result = input;

        while (pos < len)
        {
            if (result[pos] == ' ')
            {
                lastWhitespace = pos;
            }
            else if (result[pos] == '\n')
            {
                lastWhitespace = pos;
                lastBreak = pos;
            }
            else if (result[pos] == '\r')
            {
                if (result[pos + 1] == '\n')
                    pos++;
                lastWhitespace = pos;
                lastBreak = pos;
            }
            else if (textWidth(result.substr(lastBreak + 1, pos - lastBreak)) > lineLength
                && lastWhitespace != lastBreak)
            {
                result[lastWhitespace] = '\n';
                lastBreak = lastWhitespace;
            }

            pos++;
        }

        return result;
    }
}