function Map(filename)
    local SIGNATURE = 'V3MAP\0'
    local VERSION = 2
    local self = {}

    local f = plum.File(filename, 'r')
    
    local signature = f:readBlob(#SIGNATURE)    
    if signature ~= SIGNATURE then
        error(filename .. ' is an invalid map! (Incorrect signature)', 2)
    end
    
    local version = f:readInt32()
    if version ~= VERSION then
        error(filename .. ' is an unsupported version of map.', 2)
    end
    
    -- Skip vc offset.
    f:readInt32()
    
    -- String data of various use.
    self.mapName = f:readFixedString(256)
    self.vspFilename = f:readFixedString(256)
    self.musicFilename = f:readFixedString(256)
    self.renderString = f:readFixedString(256)
    self.startupScript = f:readFixedString(256)
    
    local tileset = Tileset(self.vspFilename) 
    self.tileset = tileset
    
    -- Default starting location.
    self.startX = f:readU16()
    self.startY = f:readU16()
    
    -- Layers.
    local layerCount = f:readInt32()
    self.layers = {}
    for i = 1, layerCount do
        local layer = Layer(f)
        layer.tilemap.spritesheet = tileset.tiles
        table.insert(self.layers, layer)
    end
    
    -- Size of map.
    self.width = self.layers[1].width
    self.height = self.layers[1].height
    
    -- The obstruction and zone layers.
    self.obsData = plum.verge.readTilemap8(f, self.width, self.height)
    self.obsData.spritesheet = tileset.obs
    self.zoneData = plum.verge.readTilemap16(f, self.width, self.height)
    
    -- The actual zone data.
    local zoneCount = f:readInt32()
    self.zones = {}
    for i = 1, layerCount do
        local zone = {}
        zone.name = f:readFixedString(256)
        zone.script = f:readFixedString(256)
        zone.percent = f:readU8()
        zone.delay = f:readU8()
        zone.method = f:readU8()
        table.insert(self.zones, zone)
    end
    
    -- Bah, entities. Why bother. I don't really need them.
    -- We're Done!
    f:close()
    
    return self
end

function Layer(f)
    local self = {}

    self.name = f:readFixedString(256)
    self.parallaxX = f:readDouble()
    self.parallaxY = f:readDouble()
    
    local width = f:readInt16()
    local height = f:readInt16()
    self.width = width
    self.height = height
    
    local lucent = f:readU8()
    self.opacity = (100 - lucent) * 255 / 100
    
    local tilemap = plum.verge.readTilemap16(f, width, height)
    self.tilemap = tilemap
    
    self.xOffset = 0
    self.yOffset = 0
    
    return self
end

function TileAnimation(f)
    local self = {}
    return self
end

function Tileset(filename)
    local SIGNATURE = 5264214
    local VERSION = 6
    local self = {}

    local f = plum.File(filename, 'r')
    
    local signature = f:readInt32()
    if signature ~= SIGNATURE then
        error(filename .. ' is an invalid tileset! (Incorrect signature)', 2)
    end
    
    local version = f:readInt32()
    if version ~= VERSION then
        error(filename .. ' is an unsupported version of tileset.', 2)
    end
    
    -- Tile size. Always 16.
    f:readInt32()
    -- Format. Never used.
	f:readInt32()
    
    -- Number of tiles in the tileset.
    self.tileCount = f:readInt32()
    
    local compression = f:readInt32()
    if compression ~= 1 then
        error(filename .. ' has an unsupported compression setting.', 2)
    end
    
    do
        local TEXTURE_SIZE = 512
        local TILES_PER_ROW = TEXTURE_SIZE / 16
    
        local canvas = plum.verge.readRGBCanvas(f, 16, self.tileCount * 16)
        canvas:replaceColor(plum.color.Magenta, 0)
        
        local tile = plum.Canvas(TEXTURE_SIZE, TEXTURE_SIZE)
        
        for i = 1, self.tileCount do
            local sourceY = (i - 1) * 16
            local destX = (i - 1) % TILES_PER_ROW * 16
            local destY = math.floor((i - 1) / TILES_PER_ROW) * 16
        
            canvas:blitRegion(0, sourceY, 15, sourceY + 15, destX, destY, tile, plum.blend.Opaque)
        end
        local img = plum.Image(tile)
        local spr = plum.Spritesheet(img, 16, 16)
        spr.columns = img.width / 16
        self.tiles = spr
    end
    
    local animationCount = f:readInt32()
    self.animations = {}
    for i = 1, animationCount do
        local anim = {}
        anim.name = f:readFixedString(256)
        anim.startTile = f:readInt32()
        anim.endTile = f:readInt32()
        anim.delay = f:readInt32()
        anim.mode = f:readInt32()
        table.insert(self.animations, anim)
    end
    
    self.obsCount = f:readInt32()
    do
        local TEXTURE_SIZE = 256
        local TILES_PER_ROW = TEXTURE_SIZE / 16
    
        local canvas = plum.verge.readObstructionCanvas(f, 16, self.obsCount * 16)
        
        local tile = plum.Canvas(TEXTURE_SIZE, TEXTURE_SIZE)
        
        for i = 1, self.tileCount do
            local sourceY = (i - 1) * 16
            local destX = (i - 1) % TILES_PER_ROW * 16
            local destY = math.floor((i - 1) / TILES_PER_ROW) * 16
        
            canvas:blitRegion(0, sourceY, 15, sourceY + 15, destX, destY, tile)
        end
        local img = plum.Image(tile)
        local spr = plum.Spritesheet(img, 16, 16)
        spr.columns = img.width / 16
        self.obs = spr
    end
    
    return self
end

tileset = Tileset('molasses.vsp')
map = Map('molasses.map')

fnt = plum.Font('resources/fonts/ccfont.png')
fnt:enableVariableWidth()
local TITLE = 'Test'
while not plum.key.Enter.pressed do
    plum.video:clear(plum.color.Black)
    
    for i, layer in ipairs(map.layers) do
        plum.video.opacity = layer.opacity
        layer.tilemap:blit(tileset.tiles, 0, 0, 0, 0, 20, 15)
    end
    plum.video.opacity = 127
    map.obsLayer:blit(tileset.obs, 0, 0, 0, 0, 20, 15)
    plum.video.opacity = 255
    fnt:print(0, 0, TITLE .. ' ' .. plum.timer.fps)
    plum.refresh()
    
    for i = 1, plum.timer.gap do
    end
end
