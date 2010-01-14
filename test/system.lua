require 'plumed'

function initHooks()
    -- Plumed hook.
    plumed.hookLaunch(plum.key.F10)
    plumed.hookExit(initHooks) 
end

initHooks()

tex = plum.Texture('tileset.png')

while true do
    plum.video.clear(plum.color.Black)
    tex:blit(0, 0)
    plum.refresh()
end

