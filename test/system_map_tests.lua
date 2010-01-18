local spr = plum.Spritesheet(plum.Texture('tileset.png'), 16, 16)
spr.padding = 0
spr.columns = 20

brush = plum.Tilemap(2, 2)
brush:setTile(0, 0, 28)
brush:setTile(1, 0, 29)
brush:setTile(0, 1, 48)
brush:setTile(1, 1, 49)

local bg = plum.Tilemap(70, 60)
local fg = plum.Tilemap(70, 60)

fg:solidRect(0, 0, 2, 2, 4)
fg:setTile(0, 1, 1)
fg:setTile(1, 1, 2)
fg:setTile(2, 1, 3)
fg:setTile(0, 2, 21)
fg:setTile(1, 2, 22)
fg:setTile(2, 2, 23)
fg:setTile(0, 3, 41)
fg:setTile(1, 3, 42)
fg:setTile(2, 3, 43)
fg:setTile(0, 0, 24)

for i = 0, 20, 2 do
    for j = 0, 15, 2 do
        brush:stamp(i, j, bg)
    end
end

enc = plum.data.encodeTilemap(fg)
print(enc)
fg = plum.data.decodeTilemap(enc)

local layers = { bg, fg, fg, fg }

local fnt = plum.Font('resources/fonts/ccfont.png')
fnt:enableVariableWidth()

local TILES_WIDE = plum.video.width / spr.frameWidth + 2
local TILES_HIGH = plum.video.height / spr.frameHeight + 2
while true do
    plum.video:clear(plum.color.Blue)
    for i, v in ipairs(layers) do
        v:blit(spr, 0, 0, 0, 0, TILES_WIDE, TILES_HIGH)
    end
    fnt:print(0, 0, tostring(plum.timer.fps))
    plum.refresh()
end
