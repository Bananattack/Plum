plumed.ui.class 'Widget' do
    local this_class = plumed.ui.Widget

    function this_class:__init()
        -- The position of the widget on-screen, in pixelss.
        self.position = { x = 0, y = 0 };
        -- Dimensions of the widget in pixels.
        self.dimensions = { width = 0, height = 0 }
        
        -- If this widget is somehow affected by another widget.
        self.parent = nil
        
        -- The event that occurs when the widget is clicked
        self.onClick = nil;
        -- The event that occurs when the widget is brought into focus
        self.onFocus = nil
        -- The event that occurs when the widget loses focus
        self.onBlur = nil
    end
end

