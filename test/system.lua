plum.setTitle('Giraffes IN SPACE')
tex = plum.Texture('resources/sprites/heartsprite.png')
fnt = plum.Font('resources/fonts/ccfont.png')
fnt:enableVariableWidth()

print("width = " .. tex.width .. "; height = " .. tex.height)

time = plum.timer.time

while true do
    plum.video.verticalGradientRect(0, 0, plum.video.screenWidth, plum.video.screenHeight, plum.color.rgb(0x33, 0x66, 0xcc), plum.color.Magenta)
    tex:blit(50, 50)
    
    fnt:print(5, 5, "FPS: " .. plum.timer.fps);
    
    plum.refresh()
end
