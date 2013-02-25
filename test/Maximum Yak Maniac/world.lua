do local Self = {}
    function Grass(color, xofs, yofs, height, parallax)
        local self = setmetatable({}, {__index = Self})
        self.color = color
        self.xofs = xofs
        self.yofs = yofs
        self.parallax = parallax
        self.height = height
        self.texture = plum.Image(resource.image.grass.canvas)
        self.texture.canvas:replaceColor(plum.color.Cyan, color)

        local r, g, b = plum.color.channels(color)
        self.texture.canvas:replaceColor(plum.color.Black, plum.color.rgb(r + 10, g + 10, b + 10))
        self.texture:refresh()
        return self
    end

    function Self:blit(x, y)
        x = (x + self.xofs) * self.parallax
        y = y + self.yofs
        local w = self.texture.width
        local sx = (x + w) % w
        for i = -1, plum.screen.width / w + 2 do
            self.texture:blit(sx + (i * w), y - self.texture.height)
        end
        plum.screen:solidRect(0, y, plum.screen.width, y + self.height, self.color)
    end
end

do local Self = {}
    local CameraHorizontalBorder = 200

    local GrassFurthestColor = plum.color.rgb(0x24, 0x9B, 0x7F)
    local GrassFarColor = plum.color.rgb(0x44, 0xAB, 0x5F)
    local GrassNearColor = plum.color.rgb(0x77, 0xBD, 0x3F)
    local GrassNearestColor = plum.color.rgb(0x99, 0xCD, 0x0F)

    function World()
        local self = setmetatable({}, {__index = Self})
        self.x = 0
        self.backgroundY = 160
        self.floorY = self.backgroundY + 200
        
        self.sky = Sky()
        self.grass = {
            Grass(GrassFurthestColor, 0, 120 + self.backgroundY, 50, -0.5);
            Grass(GrassFarColor, 80, 160 + self.backgroundY, 50, -1);
            Grass(GrassNearColor, 160, 200 + self.backgroundY, 50, -1.2);
            Grass(GrassNearestColor, 240, 240 + self.backgroundY, 2000, -2);
        };
        self.sprites = {}
        
        self.buildingCount = 0
        self.spawnOffset = plum.screen.width + 50
        
        self.gameOver = false
        
        return self
    end
    
    function Self:addBuilding()
        if self.spawnOffset > plum.screen.width * 5 and math.random() < 0.2 then
            for i = 1, 3 do
                if math.random() < 0.7 then
                    table.insert(world.sprites, Roids(world.x + math.random(0, plum.screen.width), -math.random(100, 200), math.random() > 0.7))
                else
                    table.insert(world.sprites, Bomb(world.x + math.random(0, plum.screen.width), -math.random(0, 200)))
                end
            end
        end

        if self.spawnOffset > plum.screen.width * 5 and math.random() < 0.2 then
            table.insert(world.sprites, Bomb(self.spawnOffset, math.random(1, 10)))
        elseif math.random() < 0.2 then
            table.insert(world.sprites, Roids(self.spawnOffset, math.random(1, 10), self.spawnOffset > plum.screen.width * 5 and math.random() > 0.7))
        elseif math.random() < 0.3 then
            table.insert(world.sprites, Balloon(self.spawnOffset, self.floorY - 240, self.floorY - 120))
        elseif self.spawnOffset > plum.screen.width * 8 and math.random() < 0.25 and not self.gameOver then
            if math.random() < 0.15 then
                table.insert(world.sprites, Truck(self.spawnOffset + 200, self.floorY - 50, 'left'))
            else
                table.insert(world.sprites, Truck(world.x - self.spawnOffset, self.floorY - 50, 'right'))
            end
        else
            self.buildingCount = self.buildingCount + 1
            table.insert(self.sprites, Building(self.spawnOffset, self.floorY))
        end
        self.spawnOffset = self.spawnOffset + math.random(110, 250)
    end
    
    function Self:register()
        players = {
            Player(80, self.floorY, controls[1], PlayerOneColor);
            Player(plum.screen.width - 80, self.floorY, controls[2], PlayerTwoColor);
        }
        
        players[1].z = 1
        players[2].z = 2
        players[2].timer = players[1].timer
    
        table.insert(self.sprites, players[1])
        table.insert(self.sprites, players[2])
        table.insert(self.sprites, Building(plum.screen.width / 2, self.floorY))

        table.insert(renderList, methodPointer(self, 'render'))
        table.insert(updateList, methodPointer(self, 'update'))
    end
    
    function Self:drawHUD()
        local fnt
        local t = math.max(players[1].timer, 0)
        local t2 = math.max(players[2].timer, 0)
        
        if playerCount > 1 then
            plum.screen:solidRect(1, 10, 6, 50, PlayerOneColor)
            resource.font.plain:print(10, 10, tostring(1000000000 + players[1].score):sub(2, 10))
            
            fnt = players[1].healCounter > 0 and resource.font.bigGreen or (players[1].hurtCounter > 0 and resource.font.bigRed or resource.font.big)
            fnt:print(10, 20, math.floor(t / 100 / 60) .. ":" .. tostring(t / 100 % 60 + 100):sub(2, 3))
            
            if players[1].scoreCounter > 0 then
                resource.font.bigYellow:print(10, 50, "+" .. tostring(players[1].scoreCounter) .. "!")
            end

            resource.font.plain:print(10, 40, "Player 1")
            resource.font.plain:printRight(plum.screen.width - 10, 40, "Player 2")
        end
            
        plum.screen:solidRect(plum.screen.width - 1, 10, plum.screen.width - 6, 50, PlayerTwoColor)
        resource.font.plain:printRight(plum.screen.width - 10, 10,  tostring(1000000000 + players[2].score):sub(2, 10))
        
        fnt = players[2].healCounter > 0 and resource.font.bigGreen or (players[2].hurtCounter > 0 and resource.font.bigRed or resource.font.big)
        fnt:printRight(plum.screen.width - 10, 20, math.floor(t2 / 100 / 60) .. ":" .. tostring(t2 / 100 % 60 + 100):sub(2, 3))
        if players[2].scoreCounter > 0 then
            resource.font.bigYellow:printRight(plum.screen.width - 10, 50, "+" .. tostring(players[2].scoreCounter) .. "!")
        end
    end
    
    function Self:render()
        local grass = self.grass
        plum.screen:solidRect(0, 0, plum.screen.width, plum.screen.height, plum.color.rgb(0x1F, 0xD1, 0xE0))
        self.sky:renderLayer(1)
        
        grass[1]:blit(self.x, 0)
        grass[2]:blit(self.x, 0)
        grass[3]:blit(self.x, 0)

        local sprites = self.sprites
        for idx, s in ipairs(sprites) do
            s.z = s.z or 0
            s.y = s.y or 0
            s.index = idx
        end

        table.sort(sprites, function(ls, rs)
            if ls.z == rs.z then
                return ls.index < rs.index
            end
            return ls.z < rs.z
        end)

        for idx, s in ipairs(sprites) do
            s:render()
        end
        
        grass[4]:blit(self.x, 0)
        self.sky:renderLayer(2)
        
        if self.gameOver then
            plum.screen.opacity = 127
            plum.screen:solidRect(0, 0, plum.screen.width, plum.screen.height, plum.color.Black)
            plum.screen.opacity = 255
            resource.font.bigYellow:printCenter(plum.screen.width / 2, plum.screen.height / 2 - 30, "GAME OVER")
            if playerCount == 2 then
                if players[1].score > players[2].score then
                    resource.font.bigGreen:printCenter(plum.screen.width / 2, plum.screen.height / 2, "PLAYER 1 WINS")
                elseif players[1].score == players[2].score then
                    resource.font.big:printCenter(plum.screen.width / 2, plum.screen.height / 2, "TIE")
                else
                    resource.font.bigRed:printCenter(plum.screen.width / 2, plum.screen.height / 2, "PLAYER 2 WINS")
                end
            end
            
            plum.screen.opacity = 127 + math.sin(math.rad(plum.timer.time) * 3) * 100
            resource.font.big:printCenter(plum.screen.width / 2, plum.screen.height / 2 + 30, "Press Enter")
            plum.screen.opacity = 255
        end
        self:drawHUD()
    end
    
    function Self:checkGameOver()
        for i, player in ipairs(players) do
            if plum.key['0'].pressed then
                player.timer = 0
            end
            if player.timer > 0 then
                return false
            end
        end
        return true
    end
    
    function Self:cameraCanMoveForward()
        a = nil
        local b = 0
        for i, player in ipairs(players) do
            if player.timer > 0 then
                b = player.x
                if player.x < self.x + plum.screen.width - CameraHorizontalBorder then
                    return false
                end
            else
                b = player.x - player.timer
            end
            if not player.offscreen then
                a = a and math.min(a, b) or b
            end
        end
        return a
    end
    
    function Self:update()
        if self.gameOver and plum.key.Enter.pressed then
            plum.key.Enter.pressed = false
            intro()
            return
        end
    
        self.x = self.x + 0.1
        self.sky:update()
        for i, sprite in ipairs(self.sprites) do
            sprite:update()
        end
    
        local a = self:cameraCanMoveForward()
        if a then
            dx = a - plum.screen.width + CameraHorizontalBorder
            if self.x < dx then
                self.x = self.x + 5
                if self.x > dx then
                    self.x = dx
                end
            end
        end
        
        if self.spawnOffset < self.x + plum.screen.width + 50 then
            self.spawnOffset = self.x + plum.screen.width + 50
        end
        
        if self:checkGameOver() then
            self.gameOver = true
        end
        
        -- Remove any sprites newly flagged for disposal, while avoiding
        -- the undefined behaviour that occurs if you
        -- you delete while iterating
        local i  = 1
        while i <= #self.sprites do
            if self.sprites[i].dispose then
                if self.sprites[i].isBuilding then
                    self.buildingCount = self.buildingCount - 1
                end
                table.remove(self.sprites, i)
            else
                i = i + 1
            end
        end
        
        if self.buildingCount < 4 then
            self:addBuilding()
        end
    end
end


