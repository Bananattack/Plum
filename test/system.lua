plum.setTitle('Giraffes IN SPACE')
tex = plum.Texture('resources/sprites/heartsprite.png')

print("width = " .. tex.width .. "; height = " .. tex.height)

time = plum.timer.time
while true do
    
    plum.video.solidRect(0, 0, plum.video.screenWidth, plum.video.screenHeight, plum.color.rgb(0x33, 0x66, 0xcc))
    
    plum.video.blendMode = plum.blend.Add
    --print(plum.video.blendMode)
    plum.video.opacity = 2
    tex:blit(5, 5)
    plum.video.opacity = 255
    plum.video.blendMode = plum.blend.Alpha
    
    if plum.timer.time - time > 100 then
        time = plum.timer.time
        plum.setTitle("FPS" .. plum.timer.fps)
    end
    
    plum.refresh()
end
