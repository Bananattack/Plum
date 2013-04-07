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
            city = plum.Song('resources/Jiggy.it')
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
    plum.screen:clear(0, 0, plum.screen.width, plum.screen.height, 0)
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
    plum.screen.title = Title .. ' ' .. plum.timer.fps
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
        x = {20, plum.screen.width - resource.image.yak.idle.width - 20},
        y = {120, 120},
    }

    while not plum.key.Enter.pressed do
        plum.screen:clear(0, 0, plum.screen.width, plum.screen.height, plum.color.rgb(20 + math.sin(math.rad(plum.timer.time) / 3) * 20, 0, 20 + math.cos(math.rad(plum.timer.time) / 3) * 20))
        

        if playerCount == 1 then
            plum.screen.opacity = 0
        end        
        plum.screen:clear(1, 10, 6, 50, PlayerOneColor)
        resource.font.plain:print(10, 10, "Controls:")
        resource.font.plain:print(10, 20, "A/D = Move")
        resource.font.plain:print(10, 30, "W = Jump")
        resource.font.plain:print(10, 40, "S = Eat")
        plum.screen.opacity = 255

        --[[resource.font.plain:print(10, 20, "Left/Right = Move")
        resource.font.plain:print(10, 30, "X = Jump")
        resource.font.plain:print(10, 40, "Square = Eat")]]
        
        plum.screen:clear(plum.screen.width - 6, 10, plum.screen.width - 1, 50, PlayerTwoColor)
        resource.font.plain:printRight(plum.screen.width - 10, 10, "Controls:")
        resource.font.plain:printRight(plum.screen.width - 10, 20, "Left/Right = Move")
        resource.font.plain:printRight(plum.screen.width - 10, 30, "Up = Jump")
        resource.font.plain:printRight(plum.screen.width - 10, 40, "Down = Eat")
        --[[resource.font.plain:printRight(plum.screen.width - 10, 20, "Left/Right = Move")
        resource.font.plain:printRight(plum.screen.width - 10, 30, "X = Jump")
        resource.font.plain:printRight(plum.screen.width - 10, 40, "Square = Eat")]]

        resource.font.bigYellow:printCenter(plum.screen.width / 2, 50, playerCount == 2 and "Two Player" or "Single Player")
        resource.font.plain:printCenter(plum.screen.width / 2, 70, playerCount == 2 and "Press 1 for Single Player" or "Press 2 for Two Player")
        if plum.key['1'].pressed then
            playerCount = 1
        elseif plum.key['2'].pressed then
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
                plum.screen.opacity = 0
            end

            local frame = player.frame[eating and 'eat' or 'idle'][i][player.direction[i]]
            if playerCount == 1 then
                frame:draw(plum.screen.width / 2 - frame.width / 2, player.y[i] + (jumping and -20 or 0))
            else
                resource.font.plain:print(player.x[i], player.y[i] - 40, "Player " .. i)
                frame:draw(player.x[i], player.y[i] + (jumping and -20 or 0))
            end

            plum.screen.opacity = 255
        end

        (plum.timer.time % 50 < 25 and resource.font.bigGreen or resource.font.bigRed):printCenter(plum.screen.width / 2, plum.screen.height / 2 - 30, "Yak Maniac")

        plum.screen.opacity = 127 + math.sin(math.rad(plum.timer.time) * 3) * 100
        resource.font.big:printCenter(plum.screen.width / 2, plum.screen.height / 2 + 30, "Press Enter")
        --resource.font.big:printCenter(plum.screen.width / 2, plum.screen.height / 2 + 30, "Press Start")
        plum.screen.opacity = 255

        resource.font.plain:printCenter(plum.screen.width / 2, plum.screen.height - 10 - resource.font.plain.cellHeight, "by Andrew G. Crowell (Music by mad)")
        
        plum.refresh()
    end
    plum.key.Enter.pressed = false

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

    plum.screen.title = Title
    intro()

    while true do
        update()
        render()
    end
end

init()
play()


