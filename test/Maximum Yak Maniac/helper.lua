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

function createSpriteFrame(img, replace, color)
    local left = plum.Image(img.canvas)
    local right = plum.Image(img.canvas)
    
    left.canvas:replaceColor(PlayerOneColor, color)
    left:refresh()
    right.canvas:replaceColor(PlayerOneColor, color)
    right.canvas:flip(true, false)
    right:refresh()
    
    return {
        left = left;
        right = right;
    }
end


function createFlipped(img)
    local result = plum.Image(img.canvas)
    result.canvas:flip(true, false)
    result:refresh()
    return result
end

function createCircleImage(xr, yr, color)
    local canvas = plum.Canvas(xr * 2 + 1, yr * 2 + 1)
    canvas:clear(0)
    canvas:fillEllipse(canvas.width / 2, canvas.height / 2, xr, yr, color, plum.blend.Opaque)
    return plum.Image(canvas)
end