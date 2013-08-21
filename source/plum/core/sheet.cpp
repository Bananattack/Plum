#include "sheet.h"

namespace plum
{
    Sheet::Sheet()
        : width(0), height(0), padding(false), columns(0), rows(0)
    {
    }

    Sheet::Sheet(int width, int height, int columns, int rows)
        : width(width),
        height(height),
        padding(false),
        columns(columns),
        rows(rows)
    {
    }

    Sheet::~Sheet()
    {
    }

    int Sheet::getWidth() const
    {
        return width;
    }

    void Sheet::setWidth(int value)
    {
        width = value;
    }

    int Sheet::getHeight() const
    {
        return height;
    }

    void Sheet::setHeight(int value)
    {
        height = value;
    }

    bool Sheet::getPadding() const
    {
        return padding;
    }

    void Sheet::setPadding(bool value)
    {
        padding = value;
    }

    int Sheet::getColumns() const
    {
        return columns;
    }

    void Sheet::setColumns(int value)
    {
        columns = value;
    }

    int Sheet::getRows() const
    {
        return rows;
    }

    void Sheet::setRows(int value)
    {
        rows = value;
    }

    bool Sheet::getFrame(int f, int& x, int& y) const
    {
        if(f < 0 || f >= rows * columns)
        {
            return false;
        }
        else
        {
            x = (f % columns) * (width + padding) + padding;
            y = (f / columns) * (height + padding) + padding;
            return true;
        }
    }
}