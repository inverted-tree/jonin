local utils = require("src.utils")

local project = {
	cwd = os.getenv("JONIN_WORKING_DIRECTORY"),
	targets = {},
	ninjaFile = "build.ninja",
}

local function loadModule(moduleName)
	local mod, err = utils.prequire(moduleName)
	if not mod then
		error(err)
	end
	return mod
end

local function createTarget(name, opts)
	loadModule("src.generators")

	local target = {
		filePath = project.ninjaFile,
	}

	target.cc = opts.cc or "cc"

	target.compileRule = Rule.new("compile", target.cc .. " $cflags -c $in -o $out")
	target.linkRule = Rule.new("link", target.cc .. " $in -o $out")

	if opts.cflags then
		target.flags = Binding.new("cflags", opts.cflags)
	end

	target.files = utils.parseFilePaths(opts.files)

	local ofiles = {}
	target.compileStmts = {}
	for _, file in ipairs(target.files) do
		local ofile, _ = string.gsub(file, "%.c", ".o")
		table.insert(ofiles, ofile)
		table.insert(target.compileStmts, BuildStatement.new(target.compileRule, file, ofile))
	end

	target.linkStmts = {}
	table.insert(target.linkStmts, BuildStatement.new(target.linkRule, table.concat(ofiles, " "), name))

	local writer = loadModule("src.writer")
	writer.writeFile(target)
end

return {
	createTarget = createTarget,
}
