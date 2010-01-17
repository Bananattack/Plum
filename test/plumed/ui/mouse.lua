plumed.ui.class 'Mouse' do
    local this_class = plumed.ui.Mouse
    local cursorImage = plum.Image('plumed/resources/images/mouse_cursor.png')

    function this_class:__init()
        self.lastX = 0
        self.lastY = 0
    end
    
    function this_class:addToEditor()
        self.renderHandle = plumed.addRender(plumed.methodPointer(self, this_class.render), 2)
        self.updateHandle = plumed.addUpdate(plumed.methodPointer(self, this_class.update))
    end
    
    function this_class:removeFromEditor()
        plumed.removeRender(self.renderHandle)
        plumed.removeUpdate(self.updateHandle)
    end
    
    function this_class:render()
        cursorImage:blit(self.x, self.y)
    end
    
    function this_class:update()
        self.lastX = self.x
        self.lastY = self.y
    end
    
    this_class._property('x', function() return plum.mouse.x end)
    this_class._property('y', function() return plum.mouse.y end)
end