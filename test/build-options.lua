-- This build script serves as a reference to write your own build-options.lua script.

-- Requiring this module is always necessary
require("jonin")

-- This function defines a new build target:
Target(
	"HelloWorld", -- name of the target
	{
		language = "c", -- specify the language (optional, only influences which files will be included)
		cc = "clang", -- set the compiler (optional, defaults to 'cc')
		cflags = { -- set compile flags to be used by your compiler
			"-Wall",
			"-Wextra",
		},
		files = { -- define the source files to use in this build (file and dir globbing with '*'/'**' is supported)
			"test/**/src",
		},
	}
)
