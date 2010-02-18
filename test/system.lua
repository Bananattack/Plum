require 'plumed'
require 'vergemap'


function initHooks()
    -- Plumed hook.
    plumed.hookLaunch(plum.key.F10)
    plumed.hookExit(initHooks) 
end

initHooks()

img = plum.Image('tileset.png')

dir = plum.Directory('resources')
repeat
    filename = dir:next()
    print(filename)
until not filename

do
    local point = plum.Point(3, 7)
    print(tostring(point) .. ' with length (should always be 2) = ' .. #point)
    print('(' .. point.x .. ', ' .. point.y .. ')')
    print('(' .. point[1] .. ', ' .. point[2] .. ')')

    local rect = plum.Rect(5, 12, 41, 36)
    print(tostring(rect) .. ' with length (should always be 4) = ' .. #rect)
    print('(' .. rect.x .. ', ' .. rect.y .. ', ' .. rect.x2 .. ', ' .. rect.y2 .. ')')
    print('(' .. rect[1] .. ', ' .. rect[2] .. ', ' .. rect[3] .. ', ' .. rect[4] .. ')')
end

local transform = plum.Transform {
    position = plum.Point(0, 0);
    clip = plum.Rect(32, 32, 64, 64);
    scale = plum.Point(2, 2);
    pivot = plum.Point(16, 16);
    angle = 30;
    mode = plum.blend.Add;
    tint = plum.color.rgb(0xff, 0xff, 0xff, 0x99);
}

print(tostring(transform) .. ' ' .. tostring(transform.pivot))

while true do
    plum.video:clear(plum.color.Black)
    img:draw(transform)
    plum.refresh()
end

