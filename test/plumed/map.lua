plumed.map = nil
plumed.mapRenderRef = nil

plumed.class 'Map' do
    local this_class = plumed.Map
    
    function this_class:__init(data)
        self.layers = nil
        if not data then
            self.layers = {}
            --[[self:addLayer({
                    TILES_WIDE = plum.video.width / spritesheet.frameWidth;
                    TILES_HEIGHT = plum.video.height / spritesheet.frameHeight;
                    width = DEFAULT_WIDTH;
                    height = DEFAULT_HEIGHT;
                    spritesheet = spritesheet;
                    tilemap = plum.Tilemap(DEFAULT_WIDTH, DEFAULT_HEIGHT);
                }
            )]]
        else
            
        end
    end

    function this_class:addLayer(layer)
        table.insert(self.layers, layer)
    end
    
    function this_class:removeLayer(index)
        table.remove(self.layers, index)
    end
    
    function this_class:render(x, y)
        for i, layer in ipairs(self.layers) do
            layer.tilemap:blit(layer.spritesheet, x, y, 0, 0, layer.TILES_WIDE, layer.TILES_HIGH)
        end
    end

    function this_class:getTile(tx, ty, layerIndex)
        return self.layers[layerIndex].tilemap:getTile(tx, ty)
    end
    
    function this_class:setTile(tx, ty, layerIndex, tileIndex)
        self.layers[layerIndex].tilemap:setTile(tx, ty, tileIndex)
    end
    
    function this_class:rect(tx, ty, tx2, ty2, layerIndex, tileIndex)
        self.layers[layerIndex].tilemap:rect(tx, ty, tx2, ty2, tileIndex)
    end
    
    function this_class:solidRect(tx, ty, tx2, ty2, layerIndex, tileIndex)
        self.layers[layerIndex].tilemap:solidRect(tx, ty, tx2, ty2, tileIndex)
    end
    
    function this_class:line(tx, ty, tx2, ty2, layerIndex, tileIndex)
        self.layers[layerIndex].tilemap:line(tx, ty, tx2, ty2, tileIndex)
    end
end

function plumed.switchMap(map)
    if plumed.mapRenderRef then
        plum.removeRender(plumed.mapRenderRef)
    end
    plumed.map = map
    plumed.mapRenderRef = plumed.addRender(function()
        map:render(0, 0)
    end)
    
    
    local DEFAULT_WIDTH = 50
    local DEFAULT_HEIGHT = 50
    local img = plum.Image('tileset.png')
    local spr = plum.Spritesheet(img, 16, 16)
    
    spr.columns = img.width / 16
    img = nil
    
    local tilemap = plum.Tilemap(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    tilemap:solidRect(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT, 4)
    
    
    map:addLayer({
            TILES_WIDE = plum.video.width / spr.frameWidth;
            TILES_HIGH = plum.video.height / spr.frameHeight;
            width = DEFAULT_WIDTH;
            height = DEFAULT_HEIGHT;
            spritesheet = spr;
            tilemap = tilemap;
        }
    )
end
