do local Self = {}
    function Roids(x, y)
        local self = setmetatable({}, {__index = Self})
        self.frame = resource.image.roids[1]
        self.hitbox = { width = self.frame.width, height = self.frame.height }
        self.z = 50
        self.isEdible = true
        self.x = x
        self.y = y
        self.angle = math.random(0, 359)
        self.scale = math.random(80, 200) / 100
        self.amount = 600 * (self.scale / 2)
        return self
    end
    
    function Self:update()
        if self.y < 180 then
            self.y = self.y + 1
            self.angle = self.angle + 1
            if self.y > 180 then
                self.y = 180
            end
        end
        
        if self.x + self.frame.width < world.x then
            self.dispose = true
        end
    end
    
    function Self:render()
        plum.video:solidCircle(self.x - world.x + self.frame.width / 2, 180, 15, 8, plum.color.Blue)
        self.frame:rotateScaleBlit(self.x - world.x, self.y - self.frame.height + 20, self.angle, self.scale)
    end
    
    function Self:damage(damage, player)
        self.dispose = true
        player:addTime(self.amount)
        resource.sound.heal:play()
    end
end
