vergeclass 'Balloon' do
    function Balloon:__init(x, y)
        self.frame = plum.Image(randomItem(resource.image.balloon).canvas)
        self.color = plum.color.hsv(math.random(0, 359), 255, 255)
        self.frame.canvas:replaceColor(plum.color.White, self.color)
        self.frame:refresh()
        
        self.hitbox = { width = 64, height = 64 }
        self.z_index = 50
        self.is_edible = true
        self.x = x
        self.y = y
        self.base_angle = math.random(-50, 50)
        self.angle = self.base_angle
        self.timer = 0
        self.scale = math.random(50, 150) / 100
        self.score = 5000
    end
    
    function Balloon:update()
        self.timer = self.timer + 1
        self.angle = self.base_angle + math.sin(self.timer / 20) * 30
        
        if self.x + self.frame.width < world.x then
            self.dispose = true
        end
    end
    
    function Balloon:render()
        plum.video:solidCircle(self.x - world.x + self.frame.width / 2, 180, 15, 8, plum.color.Red)
        self.frame:rotateScaleBlit(self.x - world.x, self.y - self.frame.height + 100, self.angle, self.scale)
    end
    
    function Balloon:damage(damage, player)
        self.dispose = true
        table.insert(world.sprites, Particle(self.x + math.random(-20, self.frame.width + 20), self.y + math.random(0, self.frame.height)))
        table.insert(world.sprites, Particle(self.x + math.random(-20, self.frame.width + 20), self.y + math.random(0, self.frame.height)))
        if math.random(0, 100) < 65 then
            table.insert(world.sprites, Roids(world.x + math.random(0, plum.video.width), math.random(1, 10)))
        end
        player:addScore(self.score)
        resource.sound.balloon_die:play()
    end 
end