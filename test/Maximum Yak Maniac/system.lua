screen = plum.Screen(640, 480, 1)

require 'helper'
require 'sky'
require 'controls'
require 'roids'
require 'particle'
require 'balloon'
require 'bomb'
require 'building'
require 'truck'
require 'textsprite'
require 'player'
require 'world'
require 'font'

function init()
    Title = 'Maximum Yak Maniac'
    resource = {
        song = {
            city = plum.Channel('resources/Jiggy.it', true)
        };
        sound = {
            blast = {
                plum.Sound('resources/blast_1.ogg');
                plum.Sound('resources/blast_2.ogg');
                plum.Sound('resources/blast_3.ogg');
                plum.Sound('resources/blast_4.ogg');
            };
            ouch = plum.Sound('resources/explode2.ogg');
            bomb = plum.Sound('resources/explode3.ogg');
            rage = plum.Sound('resources/rage.ogg');
            heal = plum.Sound('resources/heart.ogg');
            jump = plum.Sound('resources/jump_cartoony.ogg');
            thud = plum.Sound('resources/explode.ogg');
            balloonDeath = plum.Sound('resources/hiss.ogg');
        };
        font = {
            plain = Font(plum.Image('resources/font1.png'));
            big = Font(plum.Image('resources/font_big.png'));
            bigRed = Font(plum.Image('resources/font_big_red.png'));
            bigGreen = Font(plum.Image('resources/font_big_green.png'));
            bigYellow = Font(plum.Image('resources/font_big_yellow.png'));
        };
        image = {
            yak = {
                idle = plum.Image('resources/maximum_yac_idle.png');
                eat = plum.Image('resources/maximum_yac_eat.png');
            };
            cloud = {
                plum.Image('resources/cloud_1.png');
                plum.Image('resources/cloud_2.png');
            };
            bigBuilding = {
                plum.Image('resources/big_building_1.png');
            };
            building = {
                plum.Image('resources/building_1.png');
                plum.Image('resources/building_3.png');
                plum.Image('resources/building_4.png');
                plum.Image('resources/building_5.png');
            };
            poof = {
                plum.Image('resources/poof_1.png');
                plum.Image('resources/poof_2.png');
                plum.Image('resources/poof_3.png');
                plum.Image('resources/poof_4.png');
                plum.Image('resources/poof_5.png');
                plum.Image('resources/poof_6.png');
                plum.Image('resources/poof_7.png');
            };
            boom = {
                plum.Image('resources/boom1.png');
                plum.Image('resources/boom2.png');
                plum.Image('resources/boom3.png');
                plum.Image('resources/boom4.png');
                plum.Image('resources/boom5.png');
                plum.Image('resources/boom6.png');
                plum.Image('resources/boom7.png');
            };
            roids = {
                good = {
                    plum.Image('resources/pizza.png');
                    plum.Image('resources/spaghetti.png');
                    plum.Image('resources/apple_r.png');
                    plum.Image('resources/apple_g.png');
                    plum.Image('resources/strawberry.png');
                    plum.Image('resources/watermelon.png');
                    plum.Image('resources/blueberry.png');
                };
                bad = {
                    plum.Image('resources/syringe.png');
                }
            };
            bomb = {
                plum.Image('resources/bomb.png');
                plum.Image('resources/bomb2.png');
            };
            balloon = {
                plum.Image('resources/balloon_1.png');
                plum.Image('resources/balloon_2.png');
                plum.Image('resources/balloon_3.png');
            };
            truck = {
                left = plum.Image('resources/truck.png');
                right = createFlipped(plum.Image('resources/truck.png'));
            };
            
            grass = plum.Image('resources/grass.png');
        };
    };
end

renderList = {}
updateList = {}

function render()
    screen:clear(0, 0, screen.width, screen.height, 0)
    for i, f in ipairs(renderList) do
        f()
    end

    plum.refresh()
end

function update()    
    local i = 0
    while i < plum.timer.gap do
        for _, f in ipairs(updateList) do
            f()
        end
        i = i + 1
    end
    plum.sleep(10)
    screen.title = Title .. ' ' .. plum.timer.fps
    globalInputCheck()
end

local doubleClickTimer = 0

function globalInputCheck()
    if screen.close.pressed or screen.key.Escape.pressed then
        plum.exit()
    end
    if screen.key.F11.pressed or (screen.key.LeftAlt.pressed or screen.key.RightAlt.pressed) and screen.key.Enter.pressed then
        screen.key.F11.pressed = false
        screen.key.Enter.pressed = false
        screen.windowed = not screen.windowed
    end

    if screen.key.Tilde.pressed then
        plum.timer.speed = plum.speed.Fast
    elseif screen.key.LeftShift.pressed then
        plum.timer.speed = plum.speed.Slow
    else
        plum.timer.speed = plum.speed.Normal
    end

    if screen.mouse.left.pressed then
        if doubleClickTimer and doubleClickTimer > plum.timer.time then
            screen.windowed = not screen.windowed
        else
            doubleClickTimer = plum.timer.time + 20
        end
        screen.mouse.left.pressed = false
    end
end


playerCount = 1
done = false

function intro()
    local player = {
        frame = {
            idle = {
                createSpriteFrame(resource.image.yak.idle, PlayerOneColor, PlayerOneColor),
                createSpriteFrame(resource.image.yak.idle, PlayerOneColor, PlayerTwoColor)
            },
            eat = {
                createSpriteFrame(resource.image.yak.eat, PlayerOneColor, PlayerOneColor),
                createSpriteFrame(resource.image.yak.eat, PlayerOneColor, PlayerTwoColor)
            }
        };
        direction = {'right', 'left'},
        x = {20, screen.width - resource.image.yak.idle.width - 20},
        y = {120, 120},
    }

    while not screen.key.Enter.pressed do
        screen:clear(0, 0, screen.width, screen.height, plum.color.rgb(20 + math.sin(math.rad(plum.timer.time) / 3) * 20, 0, 20 + math.cos(math.rad(plum.timer.time) / 3) * 20))
        

        if playerCount == 1 then
            screen.opacity = 0
        end        
        screen:clear(1, 10, 6, 50, PlayerOneColor)
        resource.font.plain:print(10, 10, "Controls:", screen)
        resource.font.plain:print(10, 20, "A/D = Move", screen)
        resource.font.plain:print(10, 30, "W = Jump", screen)
        resource.font.plain:print(10, 40, "S = Eat", screen)
        screen.opacity = 255
        
        screen:clear(screen.width - 6, 10, screen.width - 1, 50, PlayerTwoColor)
        resource.font.plain:printRight(screen.width - 10, 10, "Controls:", screen)
        resource.font.plain:printRight(screen.width - 10, 20, "Left/Right = Move", screen)
        resource.font.plain:printRight(screen.width - 10, 30, "Up = Jump", screen)
        resource.font.plain:printRight(screen.width - 10, 40, "Down = Eat", screen)

        resource.font.bigYellow:printCenter(screen.width / 2, 50, playerCount == 2 and "Two Player" or "Single Player", screen)
        resource.font.plain:printCenter(screen.width / 2, 70, playerCount == 2 and "Press 1 for Single Player" or "Press 2 for Two Player", screen)
        if screen.key['1'].pressed then
            playerCount = 1
        elseif screen.key['2'].pressed then
            playerCount = 2
        end

        for i = 1, 2 do
            local jumping = controls[i].up:held()
            local eating = controls[i].eat:held()
            if playerCount >= 2 - i + 1 then      
                if controls[i].left:held() then
                    player.direction[i] = 'left'
                elseif controls[i].right:held() then
                    player.direction[i] = 'right'
                end
            else
                eating = false
                jumping = false
                screen.opacity = 0
            end

            local frame = player.frame[eating and 'eat' or 'idle'][i][player.direction[i]]
            if playerCount == 1 then
                frame:draw(screen.width / 2 - frame.width / 2, player.y[i] + (jumping and -20 or 0), screen)
            else
                resource.font.plain:print(player.x[i], player.y[i] - 40, "Player " .. i, screen)
                frame:draw(player.x[i], player.y[i] + (jumping and -20 or 0), screen)
            end

            screen.opacity = 255
        end

        (plum.timer.time % 50 < 25 and resource.font.bigGreen or resource.font.bigRed):printCenter(screen.width / 2, screen.height / 2 - 30, "Yak Maniac", screen)

        screen.opacity = 127 + math.sin(math.rad(plum.timer.time) * 3) * 100
        resource.font.big:printCenter(screen.width / 2, screen.height / 2 + 30, "Press Enter", screen)
        screen.opacity = 255

        resource.font.plain:printCenter(screen.width / 2, screen.height - 10 - resource.font.plain.cellHeight, "by Andrew G. Crowell (Music by mad)", screen)
        
        plum.refresh()
        globalInputCheck()
    end
    screen.key.Enter.pressed = false

    renderList = {}
    updateList = {}
    world = World()
    world:register()

    for i, p in ipairs(players) do
        if playerCount < 2 - i + 1 then
            p.timer = 0
        end
    end
end

function play()
    resource.song.city:play()

    screen.title = Title
    intro()

    while true do
        update()
        render()
    end
end

init()
play()


