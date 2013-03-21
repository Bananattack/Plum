#ifndef PLUM_SPRITE_H
#define PLUM_SPRITE_H

namespace plum
{
    class Sheet
    {
        public:
            Sheet(int width, int height, int columns, int rows);
            ~Sheet();

            int getWidth() const;
            int getHeight() const;
            bool getPadding() const;
            int getColumns() const;
            int getRows() const;

            void setWidth(int value);
            void setHeight(int value);
            void setPadding(bool value);
            void setColumns(int value);
            void setRows(int value);

            bool getFrame(int f, int& x, int& y);

        private:
            int width, height;
            int columns, rows;
            bool padding;
    };
}

#endif