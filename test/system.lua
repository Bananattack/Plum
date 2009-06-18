plum.setTitle('Giraffes IN SPACE')
tex = plum.Texture('resources/sprites/heartsprite.png')

print("width = " .. tex.width .. "; height = " .. tex.height)

time = plum.timer.time
print(time)
while true do
    plum.video.clear(plum.color.rgb(0x33, 0x66, 0xcc))
    tex:blit(5, 5)
    
    if plum.timer.time - time > 100 then
        time = plum.timer.time
        plum.setTitle("FPS" .. plum.timer.fps)
    end
    
    plum.refresh()
end
