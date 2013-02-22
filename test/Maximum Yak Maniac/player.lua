do local Self = {}
    local Acceleration = 0.2
    local MaxSpeed = 3
    local PostEatHitMaxSpeed = 2
    local PostEatMissMaxSpeed = 5
    local EatCooldown = 10
    local JumpDuration = 60
    PostEatHitDuration = 30
    PostEatMissDuration = 200

    PlayerOneColor = plum.color.rgb(0x12, 0x78, 0x50)
    PlayerTwoColor = plum.color.rgb(0x78, 0x12, 0x64)
    function Player(x, y, controls, color)
        local self = setmetatable({}, {__index = Self})
        self.walkCounter = 0
        self.angle = 0
        self.eating = false
        self.missed = false
        self.jumping = false
        self.falling = false
        self.jumpTimer = 0
        self.eatTimer = 0
        self.postEatTimer = 0
        self.speed = 0
        self.direction = 'left'
        self.score = 0
        self.timer = 30 * 100
        self.image = {}
        self.image.idle = createSpriteFrame(resource.image.yak.idle, PlayerOneColor, color)
        self.image.eat = createSpriteFrame(resource.image.yak.eat, PlayerOneColor, color)
        self.frame = self.image.idle.left
        
        self.scoreCounter = 0
        self.scoreAccel = 0
        
        self.healCounter = 0
        self.hurtCounter = 0
        
        self.z = 1
        
        self.x = x
        self.y = y - self.frame.height
        self.headWidth = 30
        
        self.controls = controls
        self.offscreen = false
        return self
    end
    
    function Self:addScore(amount)
        self.scoreCounter = self.scoreCounter + amount
    end
    
    function Self:addTime(amount)
        resource.sound.heal:play()
        self.timer = self.timer + amount
        self.healCounter = 100
    end

    function Self:subtractTime(amount)
        if self.hurtCounter <= 0 and self.timer > 0 then
            resource.sound.ouch:play()
            self.timer = self.timer - amount
            self.hurtCounter = 200
        end
    end
    
    function Self:touches(sprite)
        if self.direction == 'right' then
            return self.x + self.frame.width > sprite.x
                and self.x + self.frame.width - self.headWidth < sprite.x + sprite.hitbox.width
                and self.y + self.frame.height + 20 > sprite.y
                and self.y < sprite.y + sprite.hitbox.height
        elseif self.direction == 'left' then
            return self.x + self.headWidth > sprite.x
                and self.x < sprite.x + sprite.hitbox.width
                and self.y + self.frame.height + 20 > sprite.y
                and self.y < sprite.y + sprite.hitbox.height
        end
    end

    function Self:updateDead()
        if self.y + self.frame.height < world.floorY then
            self.y = self.y + 3
            if self.y + self.frame.height > world.floorY then
                self.y = world.floorY - self.frame.height
            end
        end
    
        if self.controls.left:held() then
            self.direction = 'left'
            self.frame = self.image.idle[self.direction]
        elseif self.controls.right:held() and not self.eating then
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
        
        self.timer = self.timer - 5
        if self.x - self.timer >= world.x + plum.screen.width then
            self.offscreen = true
        end
    end

    function Self:update()
        self.healCounter = self.healCounter - 1
        self.hurtCounter = self.hurtCounter - 1
        if self.timer <= 0 then
            self.score = self.score + self.scoreCounter
            self.scoreCounter = 0
            self:updateDead()
            return
        end
        
        if self.scoreCounter > 0 then
            self.scoreAccel = self.scoreAccel + 1
            
            local rollSpeed = 21
            if self.scoreCounter > 100000 then
                rollSpeed = 411
            elseif self.scoreCounter > 10000 then
                rollSpeed = 211
            end
            rollSpeed = rollSpeed + 8 * math.floor(self.scoreAccel / 10)
            
            self.scoreCounter = self.scoreCounter - rollSpeed
            if self.scoreCounter <= 0 then
                self.score = self.score + rollSpeed + self.scoreCounter
                self.scoreCounter = 0
                self.scoreAccel = 0
            else
                self.score = self.score + rollSpeed
            end
        end
        
        if self.jumping then
            self.y = self.y - 3
            self.jumpTimer = self.jumpTimer - 1
            self.angle = self.angle + 9
            if self.jumpTimer == 0 then
                self.jumping = false
                self.falling = true
                self.controls.up:release()
            end
        elseif self.y + self.frame.height < world.floorY then
            self.jumping = false
            self.falling = true
            self.angle = self.angle + 9
            self.y = self.y + 2
            if self.y + self.frame.height >= world.floorY then
                self.y = world.floorY - self.frame.height
                self.falling = false
            end
        else
            self.falling = false
        end
    
        if self.controls.up:held() and not self.jumping and not self.falling then
            self.jumping = true
            self.jumpTimer = JumpDuration
            resource.sound.jump:play()
        elseif not self.controls.up:held() and self.jumping then
            self.jumping = false
            self.falling = true
            self.controls.up:release()
        end
    
        if self.controls.eat:held() and not self.eating then
            self.controls.eat:release()
            self.eating = true
            self.missed = true
            self.eatTimer = EatCooldown
            for i, sprite in ipairs(world.sprites) do
                if sprite.isEdible and self:touches(sprite) then
                    sprite:damage(1, self)
                    self.missed = false
                end
            end
        end
        
        if self.eating then
            self.frame = self.image.eat[self.direction]
            self.eatTimer = self.eatTimer - 1
            if self.eatTimer <= 0 then
                self.eating = false
                if self.missed then
                    self.speed = 0
                    self.postEatTimer = PostEatMissDuration
                else
                    self.postEatTimer = PostEatHitDuration
                end
            end
        else
            self.frame = self.image.idle[self.direction]
        end
        
        self.postEatTimer = self.postEatTimer - 1
        
        if self.controls.left:held() and not self.eating then
            self.direction = 'left'
            if self.postEatTimer > 0 and not self.jumping and not self.falling then
                self.speed = math.max(self.speed - Acceleration, self.missed and -PostEatMissMaxSpeed or -PostEatHitMaxSpeed)
            else
                self.speed = math.max(self.speed - Acceleration, -MaxSpeed)
            end
            self.walkCounter = self.walkCounter + self.speed
            if not self.jumping and not self.falling then
                self.angle = math.sin(self.walkCounter / 24) * 20
            end
        elseif self.controls.right:held() and not self.eating then
            self.direction = 'right'
            if self.postEatTimer > 0 and not self.jumping and not self.falling then
                self.speed = math.min(self.speed + Acceleration, self.missed and PostEatMissMaxSpeed or PostEatHitMaxSpeed)
            else
                self.speed = math.min(self.speed + Acceleration, MaxSpeed)
            end
            self.walkCounter = self.walkCounter + self.speed
            if not self.jumping and not self.falling then
                self.angle = math.sin(self.walkCounter / 24) * 20
            end
        else
            if not self.eating then
                self.speed = 0
            end
            self.walkCounter = self.walkCounter - 1
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
        self:clampPosition()

        self.timer = self.timer - 1
    end

    function Self:clampPosition()
        if self.x < world.x then
            self.x = world.x
        elseif self.x > world.x + plum.screen.width - 40 - self.frame.width then
            self.x = world.x + plum.screen.width - 40 - self.frame.width
        end
    end
    
    function Self:render()
        plum.screen:solidCircle(self.x - world.x + self.frame.width / 2, world.floorY, 20, 3, plum.color.rgb(0, 0, 0, 127))
        if self.hurtCounter > 0 then
            if self.hurtCounter % 8 < 4 then
                self.frame:rotateBlit(self.x - world.x, self.y, self.angle + math.random(-50, 50) * (self.hurtCounter >= 150 and 1 or 0))
            end
        else
            self.frame:rotateBlit(self.x - world.x, self.y, self.angle)
        end
    end
end
