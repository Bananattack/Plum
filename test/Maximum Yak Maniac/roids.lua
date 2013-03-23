do local Self = {}
    function Roids(x, y, bad)
        local self = setmetatable({}, {__index = Self})
        self.frame = bad and randomItem(resource.image.roids.bad) or randomItem(resource.image.roids.good)
        self.shadow = createCircleImage(15, 8, plum.color.rgb(0, 0, 0, 127))
        self.hitbox = { width = self.frame.width, height = self.frame.height }
        self.z = 50
        self.isEdible = true
        self.isBad = bad
        self.x = x
        self.y = y
        self.fallSpeed = math.random() * 3 + 2
        self.angle = math.random(0, 359)
        self.angleSpeed = (math.random(0, 100) < 50 and -1 or 1) * (math.random() + 1) * 3
        self.scale = math.random(80, 200) / 100
        self.amount = 300 * (self.scale / 2)

        self.transform = plum.Transform()

        return self
    end
    
    function Self:update()
        if self.y < world.floorY then
            self.y = self.y + self.fallSpeed
            self.angle = self.angle + self.angleSpeed
            if self.y > world.floorY then
                self.y = world.floorY
            end
        end
        
        if self.x + self.frame.width < world.x then
            self.dispose = true
        end
    end
    
    function Self:render()
        self.shadow:draw(self.x - world.x + self.frame.width / 2 - self.shadow.width / 2, world.floorY - self.shadow.height / 2)
        self.transform.angle = self.angle
        self.frame:draw(self.x - world.x, self.y - self.frame.height + 20, self.transform)
    end
    
    function Self:damage(damage, player)
        self.dispose = true
        if self.isBad then
            player.hurtCounter = player.hurtCounter - 100
            player:subtractTime(self.amount * 3)
        else
            player:addTime(self.amount)
        end

        if world.spawnOffset > plum.screen.width * 5 and math.random() < (world.spawnOffset > plum.screen.width * 15 and 0.3 or 0.1) then
            table.insert(world.sprites, Bomb(world.x + math.random(0, plum.screen.width), -math.random(50, 100)))
        end
    end
end