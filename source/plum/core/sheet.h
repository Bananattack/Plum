#ifndef PLUM_SHEET_H
#define PLUM_SHEET_H

namespace plum
{
    class Sheet
    {
        public:
            Sheet();
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

            bool getFrame(int f, int& x, int& y) const;

        private:
            int width, height;
            int columns, rows;
            bool padding;
    };
}

#endif