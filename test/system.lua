function testConfig()
    t = plum.loadConfig();
    for k, v in pairs(t) do
        print(tostring(k) .. " " .. tostring(v));
    end
end

function testFileWrite()
    local file = plum.File('test.txt', 'w')
    file:writeLine("Hey there son.");
    file:close()
end

function testFileRead()
    local file = plum.File('resources/sprites/heartsprite.sprite', 'r')
    while true do
        local line = file:readLine()
        if not line then
            break
        end
        print(line)
    end
    file:close()
end


testConfig()
testFileWrite()
testFileRead()


plum.setTitle('Giraffes IN SPACE')

--tex = plum.Texture(plum.Image('resources/sprites/heartsprite.png'))
spr = plum.Sprite(140, 80, 'resources/sprites/heartsprite.sprite');
print("Hey, here is an animation from the config table:" .. spr.config.animations.throb.left);
spr:setAnimation("throb", "left")

fnt = plum.Font('resources/fonts/ccfont.png')
fnt:enableVariableWidth()

snd = plum.Sound('resources/sounds/shot.wav');

--print("width = " .. tex.width .. "; height = " .. tex.height)

--song = plum.Song('resources/songs/zk-sf-09-boss2.xm');
--song:play()

time = plum.timer.time

while not plum.key.Escape.pressed do
    plum.video.verticalGradientRect(0, 0, plum.video.screenWidth, plum.video.screenHeight, plum.color.rgb(0x33, 0x66, 0xcc), plum.color.Magenta)


    --tex:rotateScaleBlitRegion(1, 1, 16, 16, x, y, angle, 3 + math.sin(math.rad(plum.timer.time)) * 0.25 )
    spr.scale = 3 + math.sin(math.rad(plum.timer.time)) * 0.25
    spr:blit()
    
    fnt:print(5, 5, "FPS: " .. plum.timer.fps .. " " .. tostring(plum.key.Enter.pressed))
    fnt:print(5, 5 + fnt.height, "Mouse: (" .. plum.mouse.x .. ", " .. plum.mouse.y .. ")")
    for i = 1, plum.timer.gap do
        spr.angle = spr.angle + 0.5
        spr:update()
        
        if plum.key.Enter.pressed or plum.mouse.left.pressed then
            local img = plum.Image('explosiveavatar96.png')
            img:blit(96 - math.random() * 192, 96 - math.random() * 192, spr.texture.image, plum.blend.Opaque)
            spr.texture:refresh()
        
            snd:play()
            plum.key.Enter.pressed = false
            plum.mouse.left.pressed = false
        end
        
        spr.y = spr.y + (plum.mouse.wheelPosition * 3)
        plum.mouse.wheelPosition = 0
        
        if plum.key.Left.pressed then
            spr.x = spr.x - 1
        elseif plum.key.Right.pressed then
            spr.x = spr.x + 1
        end
        
        if plum.key.Up.pressed then
            spr.y = spr.y - 1
        elseif plum.key.Down.pressed then
            spr.y = spr.y + 1
        end
    end
    plum.refresh()
end

