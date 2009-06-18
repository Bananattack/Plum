plum.setTitle('Giraffes IN SPACE')
tex = plum.Texture('resources/sprites/heartsprite.png')
print("width = " .. tex.width .. "; height = " .. tex.height)
while true do
    plum.video.clear(plum.color.rgb(0x33, 0x66, 0xcc))
    tex:blit(5, 5)
    
    plum.refresh()
end
