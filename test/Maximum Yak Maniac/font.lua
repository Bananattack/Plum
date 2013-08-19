do local Self = {}
    function Font(image, rows, columns)
        local self = setmetatable({}, {__index = Self})

        rows = rows or 5
        columns = columns or 20

        self.image = image
        self.rows = rows
        self.columns = columns
        
        -- Try to automatically detect the font size based on the border edges.
        local canvas = image.canvas
        local border = canvas:get(0, 0)
        for w = 1, canvas.width - 1 do
            if canvas:get(w, 1) == border then
                self.cellWidth = w - 1
                break
            end
        end

        for h = 1, canvas.height - 1 do
            if canvas:get(1, h) == border then
                self.cellHeight = h - 1
                break
            end
        end

        self.cellWidth = self.cellWidth or canvas.width - 1
        self.cellHeight = self.cellHeight or canvas.height - 1
        self.sheet = plum.Sheet(self.cellWidth, self.cellHeight, columns, rows)
        self.sheet.padding = true

        -- Initialize glyph widths, which might be replaced later if variable width mode is enabled.
        local width = self.cellWidth
        local widths = {}
        for i = 1, rows * columns do
            table.insert(widths, width)
        end
        self.widths = widths
        self.letterSpacing = 1

        return self
    end

    function Self:enableVariableWidth()
        local canvas = self.image.canvas
        local rows, columns = self.rows, self.columns
        local width, height = self.cellWidth, self.cellHeight

        local function columnEmpty(cell, column)
            local fx = (cell % columns) * (width + 1) + 1
            local fy = math.floor(cell / columns) * (height + 1) + 1
            for y = 0, height - 1 do
                if select(4, color.channels(canvas:get(fx + column, fy + y))) > 0 then
                    return false
                end
            end
            return true
        end

        local widths = self.widths
        widths[1] = math.floor(width * 0.6)
        for i = 2, rows * columns do
            widths[i] = 1
            for x = width - 1, 1, -1 do
                if not columnEmpty(i, x) then
                    widths[i] = x + 1
                    break
                end
            end
            if widths[i] == 1 then
                widths[i] = widths[1]
            end
        end
    end

    local function printChar(self, x, y, c, dest)
        self.image:drawFrame(self.sheet, c - 32, x, y, dest)
    end

    function Self:textWidth(s, k)
        local w = 0;
        local widths = self.widths
        local letterSpacing = self.letterSpacing

        local j = 1
        local maxWidth = 0
        for i = 1, #s do
            local c = s:byte(i)
            if c == 10 then
                if j == k then
                    return w
                else
                    maxWidth = math.max(maxWidth, w)
                    w = 0
                    j = j + 1
                end
            elseif c == 9 then
                w = w + widths[1] * 4;
            elseif c >= 32 and c < 128 then
                w = w + widths[c - 31] + letterSpacing
            end
        end

        maxWidth = math.max(maxWidth, w)
        return k and w or maxWidth
    end

    function Self:print(x, y, s, dest)
        local widths = self.widths
        local height = self.cellHeight
        local letterSpacing = self.letterSpacing

        local j = x
        for i = 1, #s do
            local c = s:byte(i)
            if c == 10 then
                j = x
                y = y + height
            elseif c == 9 then
                j = j + widths[1] * 4
            elseif c >= 32 and c < 128 then
                printChar(self, j, y, c, dest)
                j = j + widths[c - 31] + letterSpacing
            end
        end
    end

    function Self:printRight(x, y, s, dest)
        local widths = self.widths
        local height = self.cellHeight
        local letterSpacing = self.letterSpacing

        local j = x
        local k = 1
        local ofs = self:textWidth(s, k)
        for i = 1, #s do
            local c = s:byte(i)
            if c == 10 then
                j = x
                y = y + height
                k = k + 1
                ofs = self:textWidth(s, k)
            elseif c == 9 then
                j = j + widths[1] * 4
            elseif c >= 32 and c < 128 then
                printChar(self, j - ofs, y, c, dest)
                j = j + widths[c - 31] + letterSpacing
            end
        end
    end

    function Self:printCenter(x, y, s, dest)
        local widths = self.widths
        local height = self.cellHeight
        local letterSpacing = self.letterSpacing

        local j = x
        local k = 1
        local ofs = math.floor(self:textWidth(s, k) / 2)
        for i = 1, #s do
            local c = s:byte(i)
            if c == 10 then
                j = x
                y = y + height
                k = k + 1
                ofs = math.floor(self:textWidth(s, k) / 2)
            elseif c == 9 then
                j = j + widths[1] * 4
            elseif c >= 32 and c < 128 then
                printChar(self, j - ofs, y, c, dest)
                j = j + widths[c - 31] + letterSpacing
            end
        end
    end

end