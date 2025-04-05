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
		os.exit(1)
	end
	local result = handle:read("*a")
	handle:close()
	return result
end

local function toStringSeq(str)
	local result = {}
	for s in str:gmatch("[^\r\n]+") do
		if s ~= "" then
			table.insert(result, s)
		end
	end
	return result
end

local function globToFind(path)
	local base, filename = nil, nil

	base, filename = path:match("^(.-)/%*%*/([^/]+)$")
	if base and filename then
		return string.format("find %q -type f -name %q", base, filename)
	end

	base, filename = path:match("^(.-)/([^/]*%*[^/]*)$")
	if base and filename then
		return string.format("find %q -maxdepth 1 -type f -name %q", base, filename)
	end

	return string.format("find %q -type f", path)
end

local function extraceSourceFiles(files)
	local sources = {}
	for _, file in ipairs(files) do
		if file:match("%.c") then
			table.insert(sources, file)
		end
	end
	return sources
end

local function getFiles(path)
	local command = globToFind(path)
	local result = executeCommand(command)
	return extraceSourceFiles(toStringSeq(result))
end

local function parseFilePaths(paths)
	local files = {}
	for _, path in ipairs(paths) do
		join(files, getFiles(path))
	end

	return files
end

return {
	prequire = prequire,
	parseFilePaths = parseFilePaths,
}
