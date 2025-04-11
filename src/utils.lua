LANGUAGE = {
	C = "c",
	CPP = "c++",
}

local function assertNotNil(value, name)
	local msg = nil
	if value == nil then
		if name then
			msg = "'" .. name .. "'"
		else
			msg = "Argument"
		end
		error(msg .. " must not be nil.", 3)
	end
end

local function prequire(mod)
	local ok, err = pcall(require, mod)
	if not ok then
		return nil, err
	end
	return err
end

local function join(target, source)
	for _, v in ipairs(source) do
		table.insert(target, v)
	end
end

local function executeCommand(command)
	local handle = io.popen(command)
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
	assertNotNil(paths, "Paths")

	local files = {}
	for _, path in ipairs(paths) do
		join(files, getFiles(language, path))
	end

	return files
end

return {
	prequire = prequire,
	parseFilePaths = parseFilePaths,
	getLanguage = getLanguage,
}
