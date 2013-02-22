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
        plain = plum.Font('resources/font1.png');
        big = plum.Font('resources/font_big.png');
        bigRed = plum.Font('resources/font_big_red.png');
        bigGreen = plum.Font('resources/font_big_green.png');
        bigYellow = plum.Font('resources/font_big_yellow.png');
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

renderList = {}
updateList = {}

function render()
    plum.screen:solidRect(0, 0, plum.screen.width, plum.screen.height, 0)
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
    plum.setTitle(Title .. ' ' .. plum.timer.fps)
end

function intro()
    while not plum.key.Enter.pressed do
        plum.screen:solidRect(0, 0, plum.screen.width, plum.screen.height, plum.color.Black)
        plum.screen:solidRect(1, 10, 6, 50, PlayerOneColor)
        plum.screen:solidRect(plum.screen.width - 6, 10, plum.screen.width - 1, 50, PlayerTwoColor)
        resource.font.plain:print(10, 10, "Controls:")
        resource.font.plain:print(10, 20, "A/D = Move")
        resource.font.plain:print(10, 30, "W = Jump")
        resource.font.plain:print(10, 40, "S = Eat")
        --[[resource.font.plain:print(10, 20, "Left/Right = Move")
        resource.font.plain:print(10, 30, "X = Jump")
        resource.font.plain:print(10, 40, "Square = Eat")]]
        
        resource.font.plain:printRight(plum.screen.width - 10, 10, "Controls:")
        resource.font.plain:printRight(plum.screen.width - 10, 20, "Left/Right = Move")
        resource.font.plain:printRight(plum.screen.width - 10, 30, "Up = Jump")
        resource.font.plain:printRight(plum.screen.width - 10, 40, "Down = Eat")
        --[[resource.font.plain:printRight(plum.screen.width - 10, 20, "Left/Right = Move")
        resource.font.plain:printRight(plum.screen.width - 10, 30, "X = Jump")
        resource.font.plain:printRight(plum.screen.width - 10, 40, "Square = Eat")]]
        
        resource.font.bigGreen:printCenter(plum.screen.width / 2, plum.screen.height / 2 - 30, "Yak Maniac")
        resource.font.big:printCenter(plum.screen.width / 2, plum.screen.height / 2 + 30, "Press Enter")
        
        resource.font.plain:printCenter(plum.screen.width / 2, plum.screen.height - resource.font.plain.height, "by Andrew G. Crowell (Music by mad)")
        plum.refresh()
    end
    plum.key.Enter.pressed = false

    renderList = {}
    updateList = {}
    world = World()
    world:register()
end

resource.song.city:play()

--vx.SetResolution(320, 240)
plum.setTitle(Title)

intro()

while true do
    update()
    render()
end


