-- Update list stuff.
-- Add to this when you want something to be updated by plumed.
plumed._updateList = nil

function plumed.initUpdate()
    plumed._updateList = plumed.newCallbackHandleList()
end

function plumed.addUpdate(f)
    return plumed.addCallbackHandle(plumed._updateList, f)
end

function plumed.removeUpdate(i)
    plumed.removeCallbackHandle(plumed._updateList, i)
end

function plumed.update()
    for i, v in ipairs(plumed._updateList) do
        if v.active then
            v.callback()
        end
    end
end