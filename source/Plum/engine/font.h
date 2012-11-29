#pragma once

#include <string>
#include "../video/color.h"
#include "../video/blending.h"

namespace plum
{
    class Image;
    class Font
    {
        public:
            static const int FONT_COLUMNS = 20;
            static const int FONT_ROWS = 5;

            Image* image;
            int width, height;
            int letterSpacing;
            int glyphWidth[FONT_COLUMNS * FONT_ROWS];

            Font(const std::string& filename);
            ~Font();

        private:
            bool isColumnEmpty(int cell, int column);

        public:
            void enableVariableWidth();
            void printChar(int x, int y, char c, BlendMode mode, Color tint);

            void print(int x, int y, const std::string& s, BlendMode mode = BlendAlpha, Color tint = Color::White);
            void printRight(int x, int y, const std::string& s, BlendMode mode = BlendAlpha, Color tint = Color::White);
            void printCenter(int x, int y, const std::string& s, BlendMode mode = BlendAlpha, Color tint = Color::White);
            
            int lineWidth(const std::string& s, int lineIndex);
            int lineCount(const std::string& s);
            int textWidth(const std::string& s);
            int textHeight(const std::string& s);
            std::string wrapText(const std::string& input, int lineLength);
    };
}