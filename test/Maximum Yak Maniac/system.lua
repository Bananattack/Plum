require 'helper'
require 'sky'
require 'controls'
require 'roids'
require 'particle'
require 'balloon'
require 'building'
require 'player'
require 'world'

TITLE = 'Maximum Yak Maniac'
resource = {
    song = {
        city = plum.Song('resources/Jiggy.it')
    };
    sound = {
        blast = {
            plum.Sound('resources/blast_1.wav');
            plum.Sound('resources/blast_2.wav');
            plum.Sound('resources/blast_3.wav');
            plum.Sound('resources/blast_4.wav');
        };
        rage = plum.Sound('resources/rage.wav');
        heal = plum.Sound('resources/heart.wav');
        jump = plum.Sound('resources/jump_cartoony.wav');
        balloon_die = plum.Sound('resources/hiss.wav');
    };
    font = {
        plain = plum.Font('resources/font1.png');
        big = plum.Font('resources/ff6big.png');
        big_green = plum.Font('resources/ff6big_green.png');
        big_yellow = plum.Font('resources/ff6big_yellow.png');
    };
    image = {
        yak = {
            idle = plum.Texture('resources/maximum_yac_idle.png');
            eat = plum.Texture('resources/maximum_yac_eat.png');
        };
        cloud = {
            plum.Texture('resources/cloud_1.png');
            plum.Texture('resources/cloud_2.png');
        };
        big_building = {
            plum.Texture('resources/big_building_1.png');
        };
        building = {
            plum.Texture('resources/building_1.png');
            plum.Texture('resources/building_3.png');
            plum.Texture('resources/building_4.png');
            plum.Texture('resources/building_5.png');
        };
        poof = {
            plum.Texture('resources/poof_1.png');
            plum.Texture('resources/poof_2.png');
            plum.Texture('resources/poof_3.png');
            plum.Texture('resources/poof_4.png');
            plum.Texture('resources/poof_5.png');
            plum.Texture('resources/poof_6.png');
            plum.Texture('resources/poof_7.png');
        };
        roids = {
            plum.Texture('resources/syringe.png');
        };
        balloon = {
            plum.Texture('resources/balloon_1.png');
            plum.Texture('resources/balloon_2.png');
            plum.Texture('resources/balloon_3.png');
        };
        
        grass = plum.Texture('resources/grass.png');
    };
};

render_list = {}
update_list = {}

function render()
    plum.video.solidRect(0, 0, plum.video.screenWidth, plum.video.screenHeight, 0)
    for i, f in ipairs(render_list) do
        f()
    end
    plum.refresh()
end

function update()    
    local i = 0
    while i < plum.timer.gap do
        for _, f in ipairs(update_list) do
            f()
        end
        i = i + 1
    end
    plum.setTitle(TITLE .. ' ' .. plum.timer.fps)
end

function intro()
    while not plum.key.Enter.pressed do
        plum.video.solidRect(0, 0, plum.video.screenWidth, plum.video.screenHeight, plum.color.Black)
        plum.video.solidRect(1, 10, 6, 50, PLAYER_ONE_COLOR)
        plum.video.solidRect(plum.video.screenWidth - 6, 10, plum.video.screenWidth - 1, 50, PLAYER_TWO_COLOR)
        resource.font.plain:print(10, 10, "Controls:")
        resource.font.plain:print(10, 20, "A/D = Move")
        resource.font.plain:print(10, 30, "W = Jump")
        resource.font.plain:print(10, 40, "S = Eat")
        
        resource.font.plain:printRight(plum.video.screenWidth - 10, 10, "Controls:")
        resource.font.plain:printRight(plum.video.screenWidth - 10, 20, "Left/Right = Move")
        resource.font.plain:printRight(plum.video.screenWidth - 10, 30, "Up = Jump")
        resource.font.plain:printRight(plum.video.screenWidth - 10, 40, "Down = Eat")
        
        resource.font.big_green:printCenter(plum.video.screenWidth / 2, plum.video.screenHeight / 2 - 30, "Maximum Yak Maniac")
        resource.font.big:printCenter(plum.video.screenWidth / 2, plum.video.screenHeight / 2 + 30, "Press Enter")
        
        resource.font.plain:printCenter(plum.video.screenWidth / 2, plum.video.screenHeight - resource.font.plain.height, "by Overkill (Music by mad)")
        plum.refresh()
    end
    plum.key.Enter.pressed = false

    render_list = {}
    update_list = {}
    world = World()
    manageWithEngine(world)
    players = {
        Player(80, controls[1], PLAYER_ONE_COLOR);
        Player(240, controls[2], PLAYER_TWO_COLOR);
    }
    
    players[1].z_index = 1
    players[2].z_index = 2
    players[2].timer = players[2].timer
    
    table.insert(world.sprites, players[1])
    table.insert(world.sprites, players[2])
end

function autoexec()
    resource.song.city:play()

	--vx.SetResolution(320, 240)
	plum.setTitle(TITLE)

    intro()

    while true do
        render()
        update()
    end
end

autoexec()

