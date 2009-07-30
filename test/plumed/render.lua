-- Render list stuff.
-- These functions are called when drawing occurs.
-- Z indexes are used to modify render priorities of things within the list.
plumed._renderList = nil
plumed._renderListOrder = nil

function plumed.initRender()
    plumed._renderList = plumed.newCallbackHandleList()
    plumed._renderListOrder = {}
end

function plumed.addRender(f, z_index)
    local i = plumed.addCallbackHandle(plumed._renderList, f)
    plumed._renderList[i].z_index = z_index or 1
    
    table.insert(plumed._renderListOrder, i)
end

function plumed.removeRender(i)
    plumed.removeCallbackHandle(plumed._renderList, i)
    for k, v in ipairs(plumed._renderListOrder) do
        if v == i then
            table.remove(plumed._renderListOrder, k)
        end
    end
end

function plumed.render()
    table.sort(plumed._renderListOrder,
        function(ls, rs)
            return plumed._renderList[ls].z_index < plumed._renderList[rs].z_index
        end
    )
    for _, i in ipairs(plumed._renderListOrder) do
        if plumed._renderList[i].active then
            plumed._renderList[i].callback()
        end
    end
end