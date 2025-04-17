local asserts = require("src.assertions")

local macros = {
	build = {
		action = function()
			local cmd = "ninja"
			os.execute(cmd)
		end,
		desc = "Build the project",
	},

	run = {
		action = function()
			local cwd = os.getenv("JONIN_WORKING_DIRECTORY")
			local cmd = "ninja && find " .. cwd .. "/build -type f -exec {} \\;"
			os.execute(cmd)
		end,
		desc = "Build and run the project",
	},

	clean = {
		action = function()
			local cmd = "ninja -t clean"
			os.execute(cmd)
		end,
		desc = "Clean up build files",
	},
}

local function createMacro(name, fun, desc)
	asserts.isNullaryFunction(fun, string.format("Macro '%s' must be a parameterless function.", name))
	macros[name] = {
		action = fun,
		desc = desc,
	}
end

return { macros = macros, createMacro = createMacro }
