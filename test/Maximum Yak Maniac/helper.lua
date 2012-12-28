function methodPointer(self, meth)
    return function(...)
        return self[meth](self, ...)
    end
end

function randomItem(t)
    if #t == 0 then
        return nil
    else
        return t[math.random(1, #t)]
    end
end

function createSpriteFrame(tex, replace, color)
    local tex_left = plum.Image(tex.canvas)
    local tex_right = plum.Image(tex.canvas)
    
    tex_left.canvas:replaceColor(PLAYER_ONE_COLOR, color)
    tex_left:refresh()
    tex_right.canvas:replaceColor(PLAYER_ONE_COLOR, color)
    tex_right.canvas:flip(true, false)
    tex_right:refresh()
    
    return {
        left = tex_left;
        right = tex_right;
    }
end
