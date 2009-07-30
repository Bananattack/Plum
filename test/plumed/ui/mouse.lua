plumed.class 'Mouse' do
    local this_class = plumed.ui.Mouse
    local cursorTexture = plum.Texture('plumed/resources/images/mouse_cursor.png')

    function this_class:__init()
        self.lastX = 0
        self.lastY = 0
    end
    
    function this_class:addToEditor()
        self.renderHandle = plumed.addRender(plumed.methodPointer(self, this_class.render))
        self.updateHandle = plumed.addUpdate(plumed.methodPointer(self, this_class.update))
    end
    
    function this_class:removeFromEditor()
        plumed.removeRender(self.renderHandle)
        plumed.removeUpdate(self.updateHandle)
    end
    
    function this_class:render()
        cursorTexture:blit(self.x, self.y)
    end
    
    function this_class:update()
        self.lastX = self.x
        self.lastY = self.y
    end
    
    this_class._property('x', function() return plum.mouse.x end)
    this_class._property('y', function() return plum.mouse.y end)
end