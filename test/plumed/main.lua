
plumed.active = false
plumed._launchInput = nil
plumed._exitCallback = nil
plumed.mouse = nil

function plumed.hookLaunch(input)
    plumed._launchInput = input
    plum.hookInput(input,
        function(inp)
            plumed.launch()
            inp.pressed = false
        end
    )
end

-- For calling a function after you're done with plumed.
-- Useful for repairing all the plum engine hooks that were killed
-- when plumed was brought into focus.
function plumed.hookExit(f)
    plumed._exitCallback = f
end

function plumed.launch()
    ---- No running multiple instances, 'kay?
    if plumed.active then
        return
    end

    ---- Prepare to take over the engine.
    ---- Will kill many engine hooks, such as all input hooks.
    ---- Hijack, ho!
    -- Activate this!
    plumed.active = true
    -- Okay, let go all the input hooks the calling user had.
    plum.unhookAllInput()
    -- Hide that cursor!
    plumed._wasMouseHidden = plum.mouse.hide
    plum.mouse.hide = true
    -- Ready the render and update lists.
    plumed.initRender()
    plumed.initUpdate()

    plumed.mouse = plumed.ui.Mouse()
    plumed.mouse:addToEditor()
    
    -- The kill switch.
    plumed.addUpdate(
        function()
            if plum.key.Escape.pressed then
                plum.key.Escape.pressed = false
                plumed.active = false
            end
        end
    )
    
    plumed.switchMap(plumed.Map())
    
    ---- Main loop stuff goes here.
    while plumed.active do
        plum.video:clear(plum.color.rgb (0x00, 0x00, 0xff))
        plumed.render()
        plum.refresh()
        for i = 1, plum.timer.gap do
            plumed.update()
            -- Break updating if, at any time, the editor is set inactive.
            if not plumed.active then
                break
            end
        end
    end

    ---- Shutdown stuff!
    ---- Kill all stuff that editor shoved into the engine.
    ---- Prepare the editor so it's ready for a subsequent launch
    -- Okay, let go of all the input hooks that plumed registered.
    plum.unhookAllInput()
    -- Revert that cursor!
    plum.mouse.hide = plumed._wasMouseHidden
    
    -- Re-hook the launch input (if there is one).
    plum.hookInput(plumed._launchInput,
        function(inp)
            plumed.launch()
            inp.pressed = false
        end
    )
    -- Call the exit callback (if it exists), which will repair all their stuff.
    if plumed._exitCallback then
        plumed._exitCallback()
    end
end