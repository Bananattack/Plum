do local Self = {}
    function Sky()
        local self = setmetatable({}, {__index = Self})
        self.clouds = {{}, {}}
        self.cloudMovementSpeed = { 0.0625, 0.50 }
        self.spawnOffset = { plum.screen.width + 50, plum.screen.width + 100 }
        return self
    end
    
    function Self:renderLayer(layer)
        plum.screen.opacity = layer == 2 and 127 or 192
        for i, cloud in ipairs(self.clouds[layer]) do
            cloud.image:draw(cloud.x - world.x, cloud.y)
        end
        --resource.font.plain:PrintCenter(plum.screen.width / 2, layer * 10, tostring(#self.clouds[layer]))
        plum.screen.opacity = 255
    end
    
    function Self:update()
        -- Spawn new clouds
        for layer = 1, 2 do
            if self.spawnOffset[layer] < world.x + plum.screen.width then
                self.spawnOffset[layer] = world.x + plum.screen.width
            end
  
            -- Move clouds
            for i, cloud in ipairs(self.clouds[layer]) do
                cloud.x = cloud.x - self.cloudMovementSpeed[cloud.layer]
                if cloud.x + cloud.image.width < world.x - 10 then
                    cloud.dispose = true
                end
            end
        
            if #self.clouds[layer] < 2 then
                local c = nil
                local t = 0
                if layer == 1 then
                    c = Cloud(self.spawnOffset[layer], math.random(0,  80), layer)
                elseif layer == 2 then
                    c = Cloud(self.spawnOffset[layer], math.random(100, plum.screen.height - 80), layer)
                end
                if c then
                    if layer == 1 then
                        self.spawnOffset[layer] = self.spawnOffset[layer] + math.random(300, 600)
                    elseif layer == 2 then
                        self.spawnOffset[layer] = self.spawnOffset[layer] + math.random(600, 900)
                    end
                    table.insert(self.clouds[layer], c)
                end
            end
            
            -- Remove any clouds newly flagged for disposal, while avoiding
            -- the undefined behaviour that occurs if you
            -- you delete while iterating
            local i  = 1
            local t = self.clouds[layer]
            while i <= #t do
                if t[i].dispose then
                    table.remove(t, i)
                else
                    i = i + 1
                end
            end
        end
    end
end

do local Self = {}
    function Cloud(x, y, layer)
        local self = setmetatable({}, Self)
        self.x = x
        self.y = y
        self.image = randomItem(resource.image.cloud)
        self.dispose = false
        self.layer = layer
        return self
    end
    
    function Self:touches(cloud)
        return self.x + self.image.width > cloud.x
                and self.x < cloud.x + cloud.image.width
                and self.y + self.image.height > cloud.y
                and self.y < cloud.y + cloud.image.height
                and self.layer == cloud.layer
    end
end