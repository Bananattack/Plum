vergeclass 'Grass' do
    function Grass:__init(color, x_offset, y_offset, height, parallax)
        self.color = color
        self.x_offset = x_offset
        self.y_offset = y_offset
        self.parallax = parallax
        self.height = height
        self.texture = plum.Image(resource.image.grass.canvas)
        self.texture.canvas:replaceColor(plum.color.Black, color)
        self.texture:refresh()
    end

    function Grass:blit(x, y)
        x = (x + self.x_offset) * self.parallax
        y = y + self.y_offset
        local w = self.texture.width
        local sx = (x + w) % w
        for i = -1, plum.video.width / w + 2 do
            self.texture:blit(sx + (i * w), y - self.texture.height)
        end
        plum.video:solidRect(0, y, plum.video.width, y + self.height, self.color)
    end
end

vergeclass 'World' do
    local CAMERA_HORIZONTAL_BORDER = 240

    local GRASS_FAREST_COLOR = plum.color.rgb(0x24, 0x9B, 0x7F)
    local GRASS_FAR_COLOR = plum.color.rgb(0x44, 0xAB, 0x5F)
    local GRASS_NEAR_COLOR = plum.color.rgb(0x77, 0xBD, 0x3F)
    local GRASS_NEAREST_COLOR = plum.color.rgb(0x99, 0xCD, 0x0F)

    function World:__init()
        self.x = 0
        
        self.sky = Sky()
        self.grass = {
            Grass(GRASS_FAREST_COLOR, 0, 120, 50, -0.5);
            Grass(GRASS_FAR_COLOR, 80, 160, 50, -1);
            Grass(GRASS_NEAR_COLOR, 160, 200, 50, -1.2);
            Grass(GRASS_NEAREST_COLOR, 240, 240, 50, -2);
        };
        self.sprites = {}
        
        self.building_count = 0
        self.spawn_offset = plum.video.width + 50
        
        self.game_over = false
        
        table.insert(self.sprites, Building(160))
    end
    
    function World:addBuilding()
        if math.random(0, 100) < 15 then
            table.insert(world.sprites, Roids(self.spawn_offset, math.random(1, 10)))
        elseif math.random(0, 100) < 30 then
            table.insert(world.sprites, Balloon(self.spawn_offset, 40, 120))
        else
            self.building_count = self.building_count + 1
            table.insert(self.sprites, Building(self.spawn_offset))
        end
        self.spawn_offset = self.spawn_offset + math.random(110, 250)
    end
    
    function World:manageWithEngine()
        table.insert(render_list, methodPointer(self, 'render'))
        table.insert(update_list, methodPointer(self, 'update'))
    end
    
    function World:drawHUD()
        local fnt
        local t = math.max(players[1].timer, 0)
        local t2 = math.max(players[2].timer, 0)
        
        plum.video:solidRect(1, 10, 6, 50, PLAYER_ONE_COLOR)
        resource.font.plain:print(10, 10, tostring(1000000000 + players[1].score):sub(2, 10))
        
        fnt = players[1].heal_counter > 0 and resource.font.big_green or resource.font.big
        fnt:print(10, 20, math.floor(t / 100 / 60) .. ":" .. tostring(t / 100 % 60 + 100):sub(2, 3))
        
        if players[1].score_counter > 0 then
            resource.font.big_yellow:print(10, 50, "+" .. tostring(players[1].score_counter) .. "!")
        end
        
        plum.video:solidRect(plum.video.width - 1, 10, plum.video.width - 6, 50, PLAYER_TWO_COLOR)
        resource.font.plain:printRight(plum.video.width - 10, 10,  tostring(1000000000 + players[2].score):sub(2, 10))
        
        fnt = players[2].heal_counter > 0 and resource.font.big_green or resource.font.big
        fnt:printRight(plum.video.width - 10, 20, math.floor(t2 / 100 / 60) .. ":" .. tostring(t2 / 100 % 60 + 100):sub(2, 3))
        if players[2].score_counter > 0 then
            resource.font.big_yellow:printRight(plum.video.width - 10, 50, "+" .. tostring(players[2].score_counter) .. "!")
        end
    end
    
    function World:render()
        local grass = self.grass
        plum.video:solidRect(0, 0, plum.video.width, plum.video.height / 2 - 1, plum.color.rgb(0x1F, 0xD1, 0xE0))
        self.sky:renderLayer(1)
        
        grass[1]:blit(self.x, 0)
        grass[2]:blit(self.x, 0)

        local sprites = self.sprites
        table.sort(sprites, function(ls, rs) return ls.z_index < rs.z_index or (ls.z_index == rs.z_index and ls.y < rs. y) end)
        for idx, s in ipairs(sprites) do
            s:render()
        end
        
        grass[3]:blit(self.x, 0)
        grass[4]:blit(self.x, 0)
        self.sky:renderLayer(2)
        
        if self.game_over then
            plum.video.opacity = 127
            plum.video:solidRect(0, 0, plum.video.width, plum.video.height, plum.color.Black)
            plum.video.opacity = 255
            resource.font.big_yellow:printCenter(plum.video.width / 2, plum.video.height / 2 - 30, "GAME OVER")
            if players[1].score > players[2].score then
                resource.font.big:printCenter(plum.video.width / 2, plum.video.height / 2, "PLAYER 1 WINS")
            elseif players[1].score == players[2].score then
                resource.font.big:printCenter(plum.video.width / 2, plum.video.height / 2, "TIE")
            else
                resource.font.big:printCenter(plum.video.width / 2, plum.video.height / 2, "PLAYER 2 WINS")
            end
            resource.font.big:printCenter(plum.video.width / 2, plum.video.height / 2 + 30, "Press Enter")
        end
        self:drawHUD()
    end
    
    function World:checkGameOver()
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
    
    function World:cameraCanMoveForward()
        a = nil
        local b = 0
        for i, player in ipairs(players) do
            if player.timer > 0 then
                b = player.x
                if player.x < self.x + plum.video.width - CAMERA_HORIZONTAL_BORDER then
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
    
    function World:update()
        if self.game_over and plum.key.Enter.pressed then
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
            self.x = a - plum.video.width + CAMERA_HORIZONTAL_BORDER
        end
        
        if self.spawn_offset < self.x + plum.video.width + 50 then
            self.spawn_offset = self.x + plum.video.width + 50
        end
        
        if self:checkGameOver() then
            self.game_over = true
        end
        
        -- Remove any sprites newly flagged for disposal, while avoiding
        -- the undefined behaviour that occurs if you
        -- you delete while iterating
        local i  = 1
        while i <= #self.sprites do
            if self.sprites[i].dispose then
                if self.sprites[i].is_building then
                    self.building_count = self.building_count - 1
                end
                table.remove(self.sprites, i)
            else
                i = i + 1
            end
        end
        
        if self.building_count < 4 then
            self:addBuilding()
        end
    end
end


