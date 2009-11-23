require 'plumed'

function initHooks()
    -- Plumed hook.
    plumed.hookLaunch(plum.key.F10)
    plumed.hookExit(initHooks) 
end

initHooks()
while true do
    plum.video.clear(plum.color.Black)
    plum.refresh()
end

