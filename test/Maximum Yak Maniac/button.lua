do local Self = {}
    function Button(...)
        local self = setmetatable({}, {__index = Self})
        self.unpress = false
        self.inputs = { ... }
        return self
    end

    function Self:held()
        local pressed = false
        for i, inp in ipairs(self.inputs) do
            if inp.pressed then
                pressed = true
                break
            end
        end
        
        if not pressed then
            if self.unpress then
                self.unpress = false
            end
            return false
        elseif not self.unpress then
            return true
        end
    end

    function Self:release()
        self.unpress = true
    end
end