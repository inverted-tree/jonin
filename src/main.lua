local macros = require("src.macros")
local targets = require("src.targets")
local utils = require("src.utils")
local writer = require("src.writer")

Project = {}
Project.cwd = os.getenv("JONIN_WORKING_DIRECTORY")
Project.macros = macros.macros
Project.targets = targets.targets

local taskList = {}

local marker = nil
for i, a in ipairs(arg) do
	if utils.isBuildScript(a) then
		if Project.buildOptions == nil then
			Project.buildOptions = utils.makePath(Project.cwd, a)
			marker = i
		else
			print("[Error] You can only specify one build script.")
			utils.usage()
			os.exit(1)
		end
	end
end
if marker ~= nil then
	table.remove(arg, marker)
end

local defOptsPath = utils.getDefaultOptionsPath(Project.cwd)
if Project.buildOptions == nil then
	if utils.fileExists(defOptsPath) then
		Project.buildOptions = defOptsPath
	else
		print(string.format("[Error] No build script specified and no %q found.", defOptsPath))
		utils.usage()
		os.exit(1)
	end
else
	if not utils.fileExists(Project.buildOptions) then
		print(string.format("[Error] No build script %q found.", defOptsPath))
		utils.usage()
		os.exit(1)
	end
end

local env = setmetatable({
	Target = targets.createTarget,
	Macro = macros.createMacro,
}, { __index = _G })
local chunk = assert(loadfile(Project.buildOptions, "t", env))
if chunk then
	chunk()
else
	print(string.format("[Error] Failed to execute build script %q.\n", Project.buildOptions))
	utils.usage()
	os.exit(1)
end

for _, macro in ipairs(arg) do
	if utils.containsKey(Project.macros, macro) then
		table.insert(taskList, macro)
	else
		print(string.format("[Error] No such task macro '%s'.", macro))
		utils.usage()
		os.exit(1)
	end
end

for _, task in ipairs(taskList) do
	local macro = Project.macros[task]
	macro.action()
end

writer.writeFile(Project.cwd .. "/build.ninja", Project.targets)
