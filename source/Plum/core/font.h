#ifndef PLUM_FONT_H
#define PLUM_FONT_H

#include <string>
#include "image.h"
#include "color.h"
#include "blending.h"

namespace plum
{
    class Font
    {
        public:
            static const int FontColumns = 20;
            static const int FontRows = 5;

            Font(const std::string& filename);
            ~Font();

            int getWidth() const;
            int getHeight() const;

            void enableVariableWidth();

            void printChar(int x, int y, char c, BlendMode mode);
            void print(int x, int y, const std::string& s, BlendMode mode);
            void printRight(int x, int y, const std::string& s, BlendMode mode);
            void printCenter(int x, int y, const std::string& s, BlendMode mode);
            
            int lineWidth(const std::string& s, int lineIndex);
            int lineCount(const std::string& s);
            int textWidth(const std::string& s);
            int textHeight(const std::string& s);
            std::string wrapText(const std::string& input, int lineLength);

        private:
            bool isColumnEmpty(int cell, int column);

            Image image;
            int width, height;
            int letterSpacing;
            int glyphWidth[FontColumns * FontRows];
    };
}

#endif