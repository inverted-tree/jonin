-- This lua module serves as an interface for scripting the build process. It delegates the function calls to the corresponding implementations in 'src/main.lua'. Since 'build-options.lua' is the entry point to each build process, it needs to call 'require("jonin")'.

local jonin = require("src.main")

--- Creates a build target
-- @param name The name of the target
-- @param opts The build options for this target
function Target(name, opts)
	jonin.createTarget(name, opts)
end
