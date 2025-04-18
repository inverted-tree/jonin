local utils = require("src.utils")
require("src.generators")

local targets = {}

local function createTarget(name, opts)
	local bindings = {}
	table.insert(bindings, Binding.new("compiler", opts.cc or "cc"))
	if opts.flags then
		table.insert(bindings, Binding.new("flags", opts.flags))
	end

	local rules = {
		compile = Rule.new("compile_" .. name, opts.cc .. " $flags -c $in -o $out", table.unpack(bindings)),
		link = Rule.new("link_" .. name, opts.cc .. " $in -o $out", table.unpack(bindings)),
	}

	local files = utils.parseFilePaths(opts.language, opts.files)
	local statements = {}

	local ofiles = {}
	for _, file in ipairs(files) do
		file = string.gsub(file, "^%./", "")
		local ofile = string.gsub(file, "%.c$", ".o")
		table.insert(ofiles, ofile)
		table.insert(statements, BuildStatement.new(rules.compile, file, ofile))
	end

	table.insert(statements, BuildStatement.new(rules.link, table.concat(ofiles, " "), "build/" .. name))

	table.insert(targets, BuildTarget.new(name, rules, statements))
end

return { targets = targets, createTarget = createTarget }
