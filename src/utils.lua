local asserts = require("src.assertions")

LANGUAGE = {
	C = "c",
	CPP = "c++",
}

local function prequire(mod)
	local ok, err = pcall(require, mod)
	if not ok then
		return nil, err
	end
	return err
end

local function join(target, source)
	for _, v in pairs(source) do
		table.insert(target, v)
	end
end

local function executeCommand(command)
	local handle = io.popen(command .. " 2>/dev/null")
	if handle == nil then
		error(string.format("Could not execute command '%s'", command), 2)
	end

	local result = {}
	for line in handle:lines() do
		table.insert(result, line)
	end
	handle:close()

	return result
end

local function getLanguage(language)
	if language == "c" then
		return LANGUAGE.C
	elseif language == "c++" or language == "cpp" then
		return LANGUAGE.CPP
	end
end

local function filter(fun, list)
	local filtered = {}
	for _, element in ipairs(list) do
		if fun(element) then
			table.insert(filtered, element)
		end
	end
	return filtered
end

local function filterCFiles(files)
	local f = function(s)
		if s:match("%.c$") then
			return true
		else
			return false
		end
	end
	return filter(f, files)
end

local function filterCppFiles(files)
	local f = function(s)
		if s:match("%.cc$") or s:match("%.cpp$") or s:match("%.cxx") then
			return true
		else
			return false
		end
	end
	return filter(f, files)
end

local function splitPath(path)
	local function normalize(d)
		if d:match("^/") or d:match("^%./") then
			return d
		else
			return "./" .. d
		end
	end

	if path:match("/$") then
		return normalize(path:gsub("/+$", "")), ""
	end

	local dir, name = path:match("^(.-)([^/\\]*)$")

	if not name:find("%.") or name == "" then
		return normalize(path:gsub("/+$", "")), ""
	end

	if dir == "" then
		dir = "."
	else
		dir = path:gsub("/+$", "")
		if dir == "" then
			dir = "/"
		end
	end

	return normalize(dir), name
end

local function getFiles(language, path)
	local dir, name = splitPath(path)
	local cmd = nil
	if name == "" then
		cmd = string.format("find -L %s", dir)
		local files = executeCommand(cmd)
		if language == LANGUAGE.C then
			return filterCFiles(files)
		elseif language == LANGUAGE.CPP then
			return filterCppFiles(files)
		else
			return files
		end
	else
		cmd = string.format("find -L %s -name '%s'", dir, name)
		return executeCommand(cmd)
	end
end

local function parseFilePaths(language, paths)
	asserts.isNotNil(paths, "Paths")

	local files = {}
	for _, path in ipairs(paths) do
		join(files, getFiles(language, path))
	end

	return files
end

local function printUsage()
	print("\nUsage: $jonin [macro] <build-options.lua>\n")

	print("Available macros:")
	for k, v in pairs(Project.macros) do
		print("\t" .. k .. "\t" .. v.desc)
	end
end

local function makePath(dir, file)
	return dir .. "/" .. file
end

local function getDefaultOptionsPath(cwd)
	return makePath(cwd, "build-options.lua")
end

local function fileExists(file)
	local cmd = string.format("find %q", file)
	local result = executeCommand(cmd)
	return #result == 1 and result[1] == file
end

local function isBuildScript(file)
	return string.match(file, ".+%.lua$") and fileExists(file)
end

local function containsKey(table, key)
	for k, _ in pairs(table) do
		if k == key then
			return true
		end
	end
end

local function contains(table, value)
	for _, val in pairs(table) do
		if val == value then
			return true
		end
	end
	return false
end

return {
	fileExists = fileExists,
	isBuildScript = isBuildScript,
	prequire = prequire,
	parseFilePaths = parseFilePaths,
	getLanguage = getLanguage,
	executeCommand = executeCommand,
	contains = contains,
	usage = printUsage,
	getDefaultOptionsPath = getDefaultOptionsPath,
	makePath = makePath,
	containsKey = containsKey,
	join = join,
}
