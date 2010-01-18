vergeclass 'Player' do
    local ACCEL = 0.2
    local MAX_SPEED = 3
    local POST_EAT_MAX_SPEED = 1
    local EAT_COOLDOWN = 10
    local JUMP_DURATION = 30
    POST_EAT_HIT_DURATION = 90
    POST_EAT_MISS_DURATION = 40

    PLAYER_ONE_COLOR = plum.color.rgb(0x12, 0x78, 0x50)
    PLAYER_TWO_COLOR = plum.color.rgb(0x78, 0x12, 0x64)
    function Player:__init(x, controls, color)
        self.walk_counter = 0
        self.angle = 0
        self.eating = false
        self.jumping = false
        self.falling = false
        self.jump_timer = 0
        self.eat_timer = 0
        self.post_eat_timer = 0
        self.speed = 0
        self.direction = 'left'
        self.score = 0
        self.timer = 30 * 100
        self.image = {}
        self.image.idle = createSpriteFrame(resource.image.yak.idle, PLAYER_ONE_COLOR, color)
        self.image.eat = createSpriteFrame(resource.image.yak.eat, PLAYER_ONE_COLOR, color)
        self.frame = self.image.idle.left
        
        self.score_counter = 0
        self.score_accel = 0
        
        self.heal_counter = 0
        
        self.z_index = 1
        
        self.x = x
        self.y = 180 - self.frame.height
        self.head_width = 30
        
        self.controls = controls
        self.offscreen = false
    end
    
    function Player:addScore(amount)
        self.score_counter = self.score_counter + amount
    end
    
    function Player:addTime(amount)
        self.timer = self.timer + amount
        self.heal_counter = 100
    end
    
    function Player:touches(sprite)
        if self.direction == 'right' then
            return self.x + self.frame.width > sprite.x
                and self.x + self.frame.width - self.head_width < sprite.x + sprite.hitbox.width
                and self.y + self.frame.height + 30 > sprite.y
                and self.y < sprite.y + sprite.hitbox.height
        elseif self.direction == 'left' then
            return self.x + self.head_width > sprite.x
                and self.x < sprite.x + sprite.hitbox.width
                and self.y + self.frame.height + 30 > sprite.y
                and self.y < sprite.y + sprite.hitbox.height
        end
    end

    function Player:updateDead()
        if self.y + self.frame.height < 180 then
            self.y = self.y + 4
            if self.y + self.frame.height > 180 then
                self.y = 180 - self.frame.height
            end
        end
    
        if self.controls.left.pressed then
            self.direction = 'left'
            self.frame = self.image.idle[self.direction]
        elseif self.controls.right.pressed and not self.eating then
            self.direction = 'right'
            self.frame = self.image.idle[self.direction]
        end
    
        if self.angle % 360 < 180 then
            self.angle = self.angle + 8
            if self.angle % 360 > 180 then
                self.angle = 180
            end
        elseif self.angle % 360 > 180 then
            self.angle = self.angle - 8
            if self.angle % 360 < 180 then
                self.angle = 180
            end
        end
        
        self.timer = self.timer - 1
        if self.x - self.timer >= world.x + plum.video.width then
            self.offscreen = true
        end
    end

    function Player:update()
        self.heal_counter = self.heal_counter - 1
        if self.timer <= 0 then
            self.score = self.score + self.score_counter
            self.score_counter = 0
            self:updateDead()
            return
        end
        
        if self.score_counter > 0 then
            self.score_accel = self.score_accel + 1
            
            local roll_speed = 21
            if self.score_counter > 100000 then
                roll_speed = 411
            elseif self.score_counter > 10000 then
                roll_speed = 211
            end
            roll_speed = roll_speed + 8 * math.floor(self.score_accel / 10)
            
            self.score_counter = self.score_counter - roll_speed
            if self.score_counter <= 0 then
                self.score = self.score + roll_speed + self.score_counter
                self.score_counter = 0
                self.score_accel = 0
            else
                self.score = self.score + roll_speed
            end            
        end
        
        if self.jumping then
            self.y = self.y - 3
            self.jump_timer = self.jump_timer - 1
            self.angle = self.angle + 15
            if self.jump_timer == 0 then
                self.jumping = false
                self.falling = true
                self.controls.up.pressed = false
            end
        elseif self.y + self.frame.height < 180 then
            self.jumping = false
            self.falling = true
            self.angle = self.angle + 15
            self.y = self.y + 2
            if self.y + self.frame.height >= 180 then
                self.y = 180 - self.frame.height
                self.falling = false
            end
        else
            self.falling = false
        end
    
        if self.controls.up.pressed and not self.jumping and not self.falling then
            self.jumping = true
            self.jump_timer = JUMP_DURATION
            resource.sound.jump:play()
        elseif not self.controls.up.pressed and self.jumping then
            self.jumping = false
            self.falling = true
            self.fall_timer = 0
            self.controls.up.pressed = false
        end
    
        if self.controls.eat.pressed and not self.eating then
            self.controls.eat.pressed = false
            self.eating = true
            self.eat_timer = EAT_COOLDOWN
            for i, sprite in ipairs(world.sprites) do
                if sprite.is_edible and self:touches(sprite) then
                    sprite:damage(1, self)
                end
            end
        end
        
        if self.eating then
            self.frame = self.image.eat[self.direction]
            self.eat_timer = self.eat_timer - 1
            if self.eat_timer <= 0 then
                self.eating = false
                self.post_eat_timer = POST_EAT_MISS_DURATION
            end
        else
            self.frame = self.image.idle[self.direction]
        end
        
        self.post_eat_timer = self.post_eat_timer - 1
        
        if self.controls.left.pressed and not self.eating then
            self.direction = 'left'
            if self.post_eat_timer > 0 then
                self.speed = math.max(self.speed - ACCEL, -POST_EAT_MAX_SPEED)
            else
                self.speed = math.max(self.speed - ACCEL, -MAX_SPEED)
            end
            self.walk_counter = self.walk_counter + self.speed
            if not self.jumping and not self.falling then
                self.angle = math.sin(self.walk_counter / 24) * 20
            end
        elseif self.controls.right.pressed and not self.eating then
            self.direction = 'right'
            if self.post_eat_timer > 0 then
                self.speed = math.min(self.speed + ACCEL, POST_EAT_MAX_SPEED)
            else
                self.speed = math.min(self.speed + ACCEL, MAX_SPEED)
            end
            self.walk_counter = self.walk_counter + self.speed
            if not self.jumping and not self.falling then
                self.angle = math.sin(self.walk_counter / 24) * 20
            end
        else
            self.speed = 0
            self.walk_counter = self.walk_counter - 1
            if not self.jumping and not self.falling then            
                if self.angle % 360 < 360 then
                    self.angle = (self.angle + 1)
                    if self.angle % 360 > 0 then
                        self.angle = 0
                    end
                elseif self.angle % 360 > 0 then
                    self.angle = (self.angle - 1)
                    if self.angle % 360 < 360 then
                        self.angle = 0
                    end
                end
            end
        end
        
        self.x = self.x + self.speed
        if self.x < world.x then
            self.x = world.x
        elseif self.x > world.x + plum.video.width - self.frame.width then
            self.x = world.x + plum.video.width - self.frame.width
        end
        
        self.timer = self.timer - 1
    end
    
    function Player:render()
        plum.video:solidCircle(self.x - world.x + self.frame.width / 2, 180, 20, 3, plum.color.Black)
        self.frame:rotateBlit(self.x - world.x, self.y, self.angle)
    end
end
