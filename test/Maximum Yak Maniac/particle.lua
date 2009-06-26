vergeclass 'Particle' do
    function Particle:__init(x, y)
        self.x = x
        self.y = y
        self.dispose = false
        self.z_index = 500
        self.frame_index = 1
        self.frame = resource.image.poof[self.frame_index]
        self.timer = 0
    end

    function Particle:render()
        self.frame:blit(self.x - world.x, self.y)
    end

    function Particle:update()
        self.timer = self.timer + 1
        if self.timer > 5 then
            self.timer = 0
            if self.frame_index < 7 then
                self.frame_index = self.frame_index + 1
                self.frame = resource.image.poof[self.frame_index]
            else
                self.dispose = true
            end
        end
    end
end
