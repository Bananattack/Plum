do local Self = {}
    function Truck(x, y, dir)
        local self = setmetatable({}, {__index = Self})
        self.frame = resource.image.truck[dir]
        self.hitbox = { width = self.frame.width + 100, height = self.frame.height + 60 }
        self.smush = {x = x, y = y, hitbox = { width = 1, height = 1 }}
        self.z = 50
        self.isEdible = true
        self.dir = dir
        self.x = x
        self.y = y
        self.timer = math.random(50, 150)
        self.fallSpeed = math.random() * 3 + 2
        self.angle = 0
        self.scale = math.random(150, 200) / 100
        self.baseScale = self.scale
        self.amount = 300 * (self.scale / 2)
        self.opacity = 255
        self.derailed = false
        self.evil = math.random()

        self.fudge = math.random(200, 1000)
        return self
    end
    
    function Self:update()
        self.timer = self.timer + 1
        if self.derailed then
            self.angle = self.angle + 10
            self.x = self.x + (self.dir == 'left' and 2 or -2)
            if self.angle > 180 then
                self.angle = 180
            end
        else
            if self.dir == 'right' and self.x < world.x - self.frame.width - self.fudge then
                self.x = self.x + 1000
                if self.x > world.x - self.frame.width - self.fudge then
                    self.x = world.x - self.frame.width - self.fudge
                end
            end
            self.x = self.x - (self.dir == 'left' and 4 or -5)
            self.angle = math.sin(math.rad(self.timer) * 10) * 10 + math.cos(math.rad(self.timer / 3) ^ 2) * 10
        end

        if world.gameOver then
            self.derailed = true
        end

        if not self.derailed then
            self.smush.x = self.x + (self.dir == 'left' and 5 or self.frame.width - self.smush.hitbox.width - 5)
            self.smush.y = self.y
            for i, player in ipairs(players) do
                if player.timer > 0 and player:touches(self.smush) then
                    resource.sound.thud:play()
                    player.jumping = false
                    player.falling = true
                    player.x = player.x - (self.dir == 'left' and 70 or -70)
                    player:clampPosition()
                    player:subtractTime(500)
                    self.timer = 0
                    self.derailed = true
                end
            end
        end

        if self.angle == 180 then
            if self.timer < 50 then
                table.insert(world.sprites, Particle(self.x + self.frame.width / 2 + math.random(-200, 200), self.y - math.random(0, self.frame.height)))
            else
                self.dispose = true
                
                local bad = self.evil > (world.spawnOffset > plum.screen.width * 15 and 0.2 or 0.6)
                if bad and math.random() < 0.3 then
                    table.insert(world.sprites, Bomb(self.x + self.frame.width / 2 + math.random(-200, 100), self.y - self.frame.height / 2 - math.random(-self.frame.height / 4, self.frame.height / 4)))
                else
                    for i = 1, math.random(6, 12) do
                        table.insert(world.sprites, Roids(self.x + self.frame.width / 2 + math.random(-200, 100), self.y - self.frame.height / 2 - math.random(-self.frame.height / 4, self.frame.height / 4), bad))
                    end
                end
                if bad and math.random() < 0.4 then
                    table.insert(world.sprites, Bomb(world.x + math.random(0, plum.screen.width), -math.random(20, 50)))
                end
            end
        end
        
        if self.dir == 'left' and self.x + self.frame.width < world.x then
            self.dispose = true
        elseif self.dir == 'right' and self.x > world.x + plum.screen.width then
            self.dispose = true
        end
    end
    
    function Self:render()
        plum.screen.opacity = self.opacity
        self.frame:rotateScaleBlit(self.x - world.x + (self.derailed and math.random(-5, 5) or 0), self.y - self.frame.height + 20 + (self.derailed and math.random(-5, 5) or 0), self.angle, self.scale)
        plum.screen.opacity = 255
    end
    
    function Self:damage(damage, player)
        if not self.derailed then
            self.timer = 0
            self.derailed = true
            self.angle = 150
            self.y = self.y + 40
            self.scale = self.scale
            self.opacity = 200
            self.evil = self.evil * 0.7
        end
        randomItem(resource.sound.blast):play()
        player:addScore(10000)
        table.insert(world.sprites, Particle(self.x + math.random(-20, self.frame.width + 20), self.y + math.random(0, self.frame.height)))
        table.insert(world.sprites, Particle(self.x + math.random(-20, self.frame.width + 20), self.y + math.random(0, self.frame.height)))
        table.insert(world.sprites, Particle(self.x + math.random(-20, self.frame.width + 20), self.y + math.random(0, self.frame.height)))
    end
end
