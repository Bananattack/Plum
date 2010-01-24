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

while true do
    plum.video:clear(plum.color.Black)
    img:blit(0, 0)
    plum.refresh()
end

