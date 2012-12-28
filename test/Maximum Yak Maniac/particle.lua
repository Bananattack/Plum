do local Self = {}
    function Particle(x, y)
        local self = setmetatable({}, {__index = Self})
        self.x = x
        self.y = y
        self.dispose = false
        self.z = 500
        self.frameIndex = 1
        self.frame = resource.image.poof[self.frameIndex]
        self.timer = 0
        return self
    end

    function Self:render()
        self.frame:blit(self.x - world.x, self.y)
    end

    function Self:update()
        self.timer = self.timer + 1
        if self.timer > 5 then
            self.timer = 0
            if self.frameIndex < 7 then
                self.frameIndex = self.frameIndex + 1
                self.frame = resource.image.poof[self.frameIndex]
            else
                self.dispose = true
            end
        end
    end
end
