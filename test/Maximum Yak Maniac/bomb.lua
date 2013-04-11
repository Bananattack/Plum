do local Self = {}
    function Bomb(x, y)
        local self = setmetatable({}, {__index = Self})
        self.frame = resource.image.bomb[1]
        self.shadow = createCircleImage(15, 8, plum.color.rgb(0, 0, 0, 127))
        self.hitbox = { width = self.frame.width, height = self.frame.height }
        self.z = 50
        self.isEdible = true
        self.x = x
        self.y = y
        self.timer = math.random(50, 150)
        self.fallSpeed = math.random() * 3 + 2
        self.angle = math.random(0, 359)
        self.angleSpeed = (math.random(0, 100) < 50 and -1 or 1) * (math.random() + 1) * 3
        self.scale = math.random(80, 200) / 100
        self.baseScale = self.scale
        self.amount = 300 * (self.scale / 2)
        
        self.transform = plum.Transform()

        return self
    end
    
    function Self:update()
        if self.timer == 0 then
            resource.sound.bomb:play()
        end

        self.scale = self.baseScale + (math.sin(math.rad(self.timer) * 3) * 0.2)
        if self.x < world.x + screen.width then
            self.timer = self.timer - 1
        end

        if self.timer < 400 then
            if self.timer % 8 < 3 then
                self.frame = resource.image.bomb[1]
            else
                self.frame = resource.image.bomb[2]
            end
        else
            if self.timer % 30 < 20 then
                self.frame = resource.image.bomb[1]
            else
                self.frame = resource.image.bomb[2]
            end
        end

        if self.timer < 0 then
            self.scale = 0
            self.frame = resource.image.bomb[2]
            if self.timer > -30 then
                table.insert(world.sprites, Particle(self.x + self.frame.width / 2 + math.random(-50, 50), self.y - math.random(0, self.frame.height), 'boom'))
            else
                table.insert(world.sprites, Particle(self.x + self.frame.width / 2 + math.random(-50, 50), self.y - math.random(0, self.frame.height)))
            end
        end

        if self.y < world.floorY then
            self.y = self.y + self.fallSpeed
            self.angle = self.angle + self.angleSpeed
            if self.y > world.floorY then
                self.y = world.floorY
            end
        end
        
        if self.timer < -80 or self.x + self.frame.width < world.x then
            table.insert(world.sprites, Particle(self.x + self.frame.width / 2 + math.random(-50, 50), self.y - math.random(0, self.frame.height)))
            self.dispose = true
        end
    end
    
    function Self:render()
        self.shadow:draw(self.x - world.x + self.frame.width / 2 - self.shadow.width / 2, world.floorY - self.shadow.height / 2, screen)
        self.transform.angle = self.angle
        self.transform.scaleX = self.scale
        self.transform.scaleY = self.scale
        self.frame:draw(self.x - world.x, self.y - self.frame.height + 20, self.transform, screen)
    end
    
    function Self:damage(damage, player)
        self.timer = 0
        if self.timer > 0 then
            player.hurtCounter = player.hurtCounter - 100
        end
        player:addScore(3000)
    end
end
