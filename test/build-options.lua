-- This build script serves as a reference to write your own build-options.lua script.

-- This function defines a new build target:
Target(
	"hello_world", -- name of the target
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

-- This function defines a new macro:
Macro(
	"hello", -- name of the macro
	function() -- nullary function that performs the action
		print("Hello from a macro!")
	end
)
