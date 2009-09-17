spr = plum.Spritesheet(plum.Texture('resources/sprites/heartsprite.png'), 16, 16)
spr.padding = 1
spr.columns = 4

tilemap = plum.Tilemap(70, 60)

tilemap:solidRect(0, 0, 2, 2, 2)
tilemap:setTile(0, 0, 3)
tilemap:setTile(1, 1, 3)

while true do
    plum.video.clear(plum.color.Black)
    tilemap:blit(spr, 0, 0, 0, 0, plum.video.screenWidth / spr.frameWidth + 2, plum.video.screenHeight / spr.frameHeight + 2)
    plum.refresh()
end
