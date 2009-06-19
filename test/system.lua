plum.setTitle('Giraffes IN SPACE')

tex = plum.Texture('resources/sprites/heartsprite.png')
fnt = plum.Font('resources/fonts/ccfont.png')
fnt:enableVariableWidth()

snd = plum.Sound('resources/sounds/shot.wav');

print("width = " .. tex.width .. "; height = " .. tex.height)

song = plum.Song('resources/songs/zk-sf-09-boss2.xm');
song:play()

time = plum.timer.time

x = 0; y = 0;
angle = 0;

while not plum.key.Escape.pressed do
    plum.video.verticalGradientRect(0, 0, plum.video.screenWidth, plum.video.screenHeight, plum.color.rgb(0x33, 0x66, 0xcc), plum.color.Magenta)
    tex:rotateBlit(x, y, angle)
    
    fnt:print(5, 5, "FPS: " .. plum.timer.fps .. " " .. tostring(plum.key.Enter.pressed))
    for i = 1, plum.timer.gap do
        angle = angle + 0.5
        
        if plum.key.Enter.pressed then
            snd:play()
            plum.key.Enter.pressed = false
        end
        
        if plum.key.Left.pressed then
            x = x - 1
        elseif plum.key.Right.pressed then
            x = x + 1
        end
        
        if plum.key.Up.pressed then
            y = y - 1
        elseif plum.key.Down.pressed then
            y = y + 1
        end
    end
    plum.refresh()
end

