do local Self = {}
    function TextSprite(x, y, font, text)
        local self = setmetatable({}, {__index = Self})
        self.x = x
        self.y = y
        self.font = font
        self.text = text
        self.dispose = false
        self.z = 10000
        self.timer = 100
        return self
    end

    function Self:render()
        self.font:print(self.x - world.x, self.y, self.text, screen)
    end

    function Self:update()
        if self.timer > 0 then
            self.timer = self.timer - 1
            self.y = self.y - 2
        end
        if self.timer == 0 then
            self.dispose = true
        end
    end
end
