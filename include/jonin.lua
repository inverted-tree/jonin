-- This lua module serves as an interface for scripting the build process. It delegates the function calls to the corresponding implementations in 'src/main.lua'. Since 'build-options.lua' is the entry point to each build process, it needs to call 'require("jonin")'. This is helpful when using a Lua LSP with autocompletion.

local jonin = require("src.main")

function Project(name)
	jonin.setProjectName(name)
end

function Version(version)
	jonin.setProjectVersion(version)
end

function Target(name, opts)
	jonin.createTarget(name, opts)
end
