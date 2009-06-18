plum.setTitle('Giraffes IN SPACE')
tex = plum.Texture('resources/sprites/heartsprite.png')
fnt = plum.Font('resources/fonts/ccfont.png')
fnt:enableVariableWidth()

print("width = " .. tex.width .. "; height = " .. tex.height)

time = plum.timer.time
while true do
    plum.video.solidRect(0, 0, plum.video.screenWidth, plum.video.screenHeight, plum.color.rgb(0x33, 0x66, 0xcc))
    
    plum.video.opacity = math.sin(math.rad(plum.timer.time)) * 63 + 127
    tex:blit(50, 50)
    plum.video.opacity = 255
    
    fnt:print(5, 5, "FPS: " .. plum.timer.fps);
    
    plum.refresh()
end
