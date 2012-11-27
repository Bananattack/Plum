vergeclass 'Building' do
    function Building:__init(x)
        self.is_edible = true
        self.is_building = true
        self.is_big = math.random(0, 100) < 10
        self.has_roids = math.random(0, 100) < 20
        self.z_index = 0
        self.score = 5000
        self.frame = self.is_big and randomItem(resource.image.big_building) or randomItem(resource.image.building)
        
        self.max_health = 4
        self.health = self.max_health
        
        -- Big buildings have more max health
        -- Also less chance of containing 'roids.
        if self.is_big then
            self.has_roids = math.random(0, 100) < 10
            self.max_health = 5
            self.health = self.max_health
            -- Find an equivalent "small building" picture as you destroy further
            for i, frame in ipairs(resource.image.big_building) do
                if frame == self.frame then
                    self.small_frame = resource.image.building[i]
                end
            end
        end
        
        self.hitbox = { width = self.frame.width, height = self.frame.height }
        
        self.dispose = false
        
        
        self.x = x
        self.shake_timer = 0
        self.y = 180 - self.frame.height
    end
    
    function Building:update()
        if self.x + self.frame.width < world.x then
            self.dispose = true
        end
        if self.shake_timer > 0 then
            self.shake_timer = self.shake_timer - 1
        end
    end
    
    function Building:render()
        local x = self.x - world.x
        if self.shake_timer > 0 then
            x = x + math.random(-5, 5)
        end
        self.frame:blit(x, self.y)
    end
    
    function Building:damage(damage, player)
        self.health = math.max(self.health - damage, 0)
        table.insert(world.sprites, Particle(self.x + math.random(-20, self.frame.width - 44), self.y + math.random(0, self.frame.height)))
        table.insert(world.sprites, Particle(self.x + math.random(-20, self.frame.width - 44), self.y + math.random(0, self.frame.height)))
        table.insert(world.sprites, Particle(self.x + math.random(-20, self.frame.width - 44), self.y + math.random(0, self.frame.height)))
        if self.has_roids and math.random(0, 100) < 75 then
            table.insert(world.sprites, Roids(world.x + math.random(0, plum.video.width), math.random(1, 10)))
        end
        self.shake_timer = 80
        
        player.post_eat_timer = POST_EAT_HIT_DURATION
        randomItem(resource.sound.blast):play()
        if self.health == 0 then
            player:addScore(self.score * 5)

            if self.is_big then
                self.frame = self.small_frame
                self.is_big = false
                self.health = self.max_health
                self.hitbox = { width = self.frame.width, height = self.frame.height }
            else
                self.dispose = true
            end
        else
            player:addScore(self.score)
        end
    end
end
