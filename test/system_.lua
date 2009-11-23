require 'tests'

require 'plumed'

testConfig()
testFileWrite()
testFileRead()
testFileReadFully()

plum.setTitle('Giraffes IN SPACE')

--tex = plum.Texture(plum.Image('resources/sprites/heartsprite.png'))
spr = plum.Sprite(140, 80, 'resources/sprites/heartsprite.sprite');
print("Hey, here is an animation from the config table:" .. spr.config.animations.throb.left);
spr:setAnimation("throb", "left")

fnt = plum.Font('resources/fonts/ccfont.png')
fnt:enableVariableWidth()

img = plum.Image('resources/sprites/heartsprite.png')
print(plum.data.digestFile('resources/sprites/heartsprite.png'))
tex = plum.data.decodeTexture(plum.data.encodeImage(img))
img = nil
tileset = plum.Tileset("test.tileset")
showTiles = false
showEncodedTexture = false

function initHooks()
    -- Plumed hook.
    plumed.hookLaunch(plum.key.F10)
    plumed.hookExit(initHooks)

    plum.hookInput(plum.key['E'], 
            function(input)
                showEncodedTexture = not showEncodedTexture
                showTiles = false
                input.pressed = false
            end
        )
    plum.hookInput(plum.key['1'], 
            function(input)
                showEncodedTexture = false
                showTiles = not showTiles
                input.pressed = false
            end
        )
    plum.hookInput(plum.key['R'], 
            function(input)
                -- Replace tiles.
                tileset:replaceTiles(spr.texture)
                input.pressed = false
            end
        )    
    plum.hookInput(plum.key['S'], 
            function(input)
                -- Replace tiles.
                tileset:save("save.tileset")
                input.pressed = false
            end
        )
    
end
initHooks()


snd = plum.Sound('resources/sounds/shot.wav');

--print("width = " .. tex.width .. "; height = " .. tex.height)

song = plum.Song('ambienttest.it');
song:play()

time = plum.timer.time

while not plum.key.Escape.pressed do
    plum.video.verticalGradientRect(0, 0, plum.video.screenWidth, plum.video.screenHeight, plum.color.rgb(0x33, 0x66, 0xcc), plum.color.Magenta)
    spr.scale = 3 + math.sin(math.rad(plum.timer.time)) * 0.25
    if showTiles then
        tileset.tiles:blit(0, 0)
    elseif showEncodedTexture then
        tex:rotateScaleBlitRegion(1, 1, 16, 16, spr.x, spr.y, spr.angle, spr.scale)
    else
        spr:blit()
    end
    
    

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