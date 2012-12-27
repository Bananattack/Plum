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
            plum.Sound('resources/blast_1.ogg');
            plum.Sound('resources/blast_2.ogg');
            plum.Sound('resources/blast_3.ogg');
            plum.Sound('resources/blast_4.ogg');
        };
        rage = plum.Sound('resources/rage.ogg');
        heal = plum.Sound('resources/heart.ogg');
        jump = plum.Sound('resources/jump_cartoony.ogg');
        balloon_die = plum.Sound('resources/hiss.ogg');
    };
    font = {
        plain = plum.Font('resources/font1.png');
        big = plum.Font('resources/ff6big.png');
        big_green = plum.Font('resources/ff6big_green.png');
        big_yellow = plum.Font('resources/ff6big_yellow.png');
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
        big_building = {
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
        roids = {
            plum.Image('resources/syringe.png');
        };
        balloon = {
            plum.Image('resources/balloon_1.png');
            plum.Image('resources/balloon_2.png');
            plum.Image('resources/balloon_3.png');
        };
        
        grass = plum.Image('resources/grass.png');
    };
};

render_list = {}
update_list = {}

function render()
    plum.video:solidRect(0, 0, plum.video.width, plum.video.height, 0)
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
        plum.video:solidRect(0, 0, plum.video.width, plum.video.height, plum.color.Black)
        plum.video:solidRect(1, 10, 6, 50, PLAYER_ONE_COLOR)
        plum.video:solidRect(plum.video.width - 6, 10, plum.video.width - 1, 50, PLAYER_TWO_COLOR)
        resource.font.plain:print(10, 10, "Controls:")
        resource.font.plain:print(10, 20, "A/D = Move")
        resource.font.plain:print(10, 30, "W = Jump")
        resource.font.plain:print(10, 40, "S = Eat")
        
        resource.font.plain:printRight(plum.video.width - 10, 10, "Controls:")
        resource.font.plain:printRight(plum.video.width - 10, 20, "Left/Right = Move")
        resource.font.plain:printRight(plum.video.width - 10, 30, "Up = Jump")
        resource.font.plain:printRight(plum.video.width - 10, 40, "Down = Eat")
        
        resource.font.big_green:printCenter(plum.video.width / 2, plum.video.height / 2 - 30, "Maximum Yak Maniac")
        resource.font.big:printCenter(plum.video.width / 2, plum.video.height / 2 + 30, "Press Enter")
        
        resource.font.plain:printCenter(plum.video.width / 2, plum.video.height - resource.font.plain.height, "by Overkill (Music by mad)")
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

