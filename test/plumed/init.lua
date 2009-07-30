plumed = {}

-- Contains all the useful helper code,
-- like the 'class' function, and the method pointer wrapper, and other useful tidbits.
-- Referenced by pretty much everything.
require 'plumed.helper'

-- The GUI stuff that gives all sorts of pretty widgets
require 'plumed.ui'


-- Used by the main loop in plumed.
require 'plumed.callback_handle_list'
require 'plumed.render'
require 'plumed.update'
-- The main 'program' routine for the editor
require 'plumed.main'

