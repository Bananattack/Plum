do local Self = {}
    function Particle(x, y, category)
        local self = setmetatable({}, {__index = Self})
        self.category = category or 'poof'
        self.x = x
        self.y = y
        self.dispose = false
        self.z = 500
        self.frameIndex = 1
        self.frame = resource.image[self.category][self.frameIndex]
        self.hitbox = { width = self.frame.width, height = self.frame.height }
        self.timer = 0
        return self
    end

    function Self:render()
        self.frame:draw(self.x - world.x, self.y)
    end

    function Self:update()
        if self.category == 'boom' then
            for i, player in ipairs(players) do
                if player:touches(self) then
                    player:subtractTime(1000 + math.random(0, 500))
                end
            end
        end

        self.timer = self.timer + 1
        if self.timer > 5 then
            self.timer = 0
            if self.frameIndex < 7 then
                self.frameIndex = self.frameIndex + 1
                self.frame = resource.image[self.category][self.frameIndex]
                self.hitbox = { width = self.frame.width, height = self.frame.height }
            else
                self.dispose = true
            end
        end
    end
end
