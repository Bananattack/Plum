do local Self = {}
    function Building(x)
        local self = setmetatable({}, {__index = Self})
        self.isEdible = true
        self.isBuilding = true
        self.isBig = math.random(0, 100) < 10
        self.hasRoids = math.random(0, 100) < 20
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
        self.y = 180 - self.frame.height
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
            table.insert(world.sprites, Roids(world.x + math.random(0, plum.screen.width), math.random(1, 10)))
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
            else
                self.dispose = true
            end
        else
            player:addScore(self.score)
        end
    end
end
