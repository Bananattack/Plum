do local Self = {}
    function Building(x)
        local self = setmetatable({}, {__index = Self})
        self.isEdible = true
        self.isBuilding = true
        self.isBig = math.random() < 0.1
        self.hasRoids = math.random() < 0.7
        self.isBad = world.spawnOffset > plum.screen.width * 5 and math.random() > 0.6
        self.z = 0
        self.score = 5000
        self.frame = self.isBig and randomItem(resource.image.bigBuilding) or randomItem(resource.image.building)
        
        self.maxHealth = 4
        self.health = self.maxHealth
        
        -- Big buildings have more max health
        -- Also less chance of containing 'roids.
        if self.isBig then
            self.hasRoids = math.random(0, 100) < 10
            self.maxHealth = 5
            self.health = self.maxHealth
            -- Find an equivalent "small building" picture as you destroy further
            for i, frame in ipairs(resource.image.bigBuilding) do
                if frame == self.frame then
                    self.smallFrame = resource.image.building[i]
                end
            end
        end
        
        self.hitbox = { width = self.frame.width, height = self.frame.height }
        
        self.dispose = false
        
        
        self.x = x
        self.shakeTimer = 0
        self.y = world.floorY - self.frame.height
        return self
    end
    
    function Self:update()
        if self.x + self.frame.width < world.x then
            self.dispose = true
        end
        if self.shakeTimer > 0 then
            self.shakeTimer = self.shakeTimer - 1
        end
    end
    
    function Self:render()
        local x = self.x - world.x
        if self.shakeTimer > 0 then
            x = x + math.random(-5, 5)
        end
        self.frame:blit(x, self.y)
    end
    
    function Self:damage(damage, player)
        self.health = math.max(self.health - damage, 0)
        table.insert(world.sprites, Particle(self.x + math.random(-20, self.frame.width - 44), self.y + math.random(0, self.frame.height)))
        table.insert(world.sprites, Particle(self.x + math.random(-20, self.frame.width - 44), self.y + math.random(0, self.frame.height)))
        table.insert(world.sprites, Particle(self.x + math.random(-20, self.frame.width - 44), self.y + math.random(0, self.frame.height)))
        if self.hasRoids and math.random(0, 100) < 75 then
            if self.isBad and math.random(0, 100) < 50 then
                table.insert(world.sprites, Bomb(math.random(math.max(world.x, self.x - plum.screen.width / 3), math.min(world.x + plum.screen.width, self.x + plum.screen.width / 3)), math.random(-300, -10), self.isBad))
            else
                for i = 1, math.random(3, 5) do
                    table.insert(world.sprites, Roids(math.random(math.max(world.x, self.x - plum.screen.width / 3), math.min(world.x + plum.screen.width, self.x + plum.screen.width / 3)), math.random(-300, -10), self.isBad))
                end
            end
        end
        self.shakeTimer = 80
        
        player.postEatTimer = PostEatHitDuration
        randomItem(resource.sound.blast):play()
        if self.health == 0 then
            player:addScore(self.score * 5)

            if self.isBig then
                self.frame = self.smallFrame
                self.isBig = false
                self.health = self.maxHealth
                self.hitbox = { width = self.frame.width, height = self.frame.height }
                self.y = world.floorY - self.frame.height
            else
                self.dispose = true
            end
        else
            player:addScore(self.score)
        end
    end
end
