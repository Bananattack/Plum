-- General callback handle list stuff.
-- Basically, it's an internal list of functions.
-- But it will allow "holes" in the list, so that handles can be freed without reindexing existing entries.
-- So this essentially reuses memory when possible, and will allow removals without
-- having to worry about handles elsewhere being invalidated.
function plumed.newCallbackHandleList()
    return {}
end

function plumed.addCallbackHandle(list, f)
    for i, v in ipairs(list) do
        if not v.active then
            v.callback = f
            v.active = true
            return i
        end
    end
    table.insert(list, { callback = f, active = true })
    return #list
end

function plumed.removeCallbackHandle(list, i)
    list[i].active = false
end