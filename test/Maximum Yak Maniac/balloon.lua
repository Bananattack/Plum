do local Self = {}
    function Balloon(x, minY, maxY)
        local self = setmetatable({}, {__index = Self})
        self.frame = plum.Image(randomItem(resource.image.balloon).canvas)
        self.shadow = createCircleImage(15, 8, plum.color.rgb(0, 0, 0, 127))
        self.color = plum.color.hsv(math.random(0, 359), 255, 255)
        self.frame.canvas:replaceColor(plum.color.White, self.color)
        local r, g, b = plum.color.channels(self.color)
        self.frame.canvas:replaceColor(plum.color.Blue, plum.color.rgb(r - 50, g - 50, b - 50))
        
        self.hitbox = { width = 64, height = 64 }

        self.z = 50
        self.isEdible = true
        self.x = x
        self.y = math.random(minY, maxY)
        self.baseAngle = math.random(-50, 50)
        self.angle = self.baseAngle
        self.timer = 0
        self.scale = math.random(50, 150) / 100
        self.score = 5000

        self.transform = plum.Transform()

        return self
    end
    
    function Self:update()
        self.timer = self.timer + 1
        self.angle = self.baseAngle + math.sin(self.timer / 20) * 30
        
        if self.x + self.frame.width < world.x then
            self.dispose = true
        end
    end
    
    function Self:render()
        self.shadow:draw(self.x - world.x + self.frame.width / 2 - self.shadow.width / 2, world.floorY - self.shadow.height / 2, screen)
        self.transform.angle = self.angle
        self.frame:draw(self.x - world.x, self.y - self.frame.height + 100, self.transform, screen)
    end
    
    function Self:damage(damage, player)
        self.dispose = true
        table.insert(world.sprites, Particle(self.x + math.random(-20, self.frame.width + 20), self.y + math.random(0, self.frame.height)))
        table.insert(world.sprites, Particle(self.x + math.random(-20, self.frame.width + 20), self.y + math.random(0, self.frame.height)))
        if math.random() < 0.65 then
            table.insert(world.sprites, Roids(math.random(math.max(world.x, self.x - 20), math.min(world.x + screen.width, self.x + 20)), self.y - 30 - math.random(0, self.frame.height)))
        elseif math.random() < (world.spawnOffset > screen.width * 15 and 0.4 or 0.15) then
            table.insert(world.sprites, Bomb(math.random(math.max(world.x, self.x - 20), math.min(world.x + screen.width, self.x + 20)), self.y - 30 - math.random(0, self.frame.height)))
        end
        if math.random() < (world.spawnOffset > screen.width * 15 and 0.6 or 0.5) then
            if math.random() < 0.5 then
                table.insert(world.sprites, Truck(world.spawnOffset + 200, world.floorY - 50, 'left'))
            else
                table.insert(world.sprites, Truck(world.x - world.spawnOffset, world.floorY - 50, 'right'))
            end
        end
        player:addScore(self.score)
        resource.sound.balloonDeath:play()
    end 
end