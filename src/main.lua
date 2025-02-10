-- This will become a high-level build system for C projects

local function prequire(m)
	local ok, err = pcall(require, m)
	if not ok then
		return nil, err
	end
	return err
end

local function get_build_opts()
	local modname = "build-options"
	local opts = prequire(modname)
	if not opts then
		error("Failed to load module '" .. modname .. "'")
	end
	if type(opts) ~= "table" then
		error("Expected 'build-options' to return a table, but got " .. type(opts))
	end

	local defopts = {
		target = "main",
		files = { "src/*.c" },
		mode = "debug",
		define = {},
		cflags = {},
	}

	for key, value in pairs(defopts) do
		if opts[key] == nil then
			opts[key] = value
		end
	end

	return opts
end

local function main()
	local options = get_build_opts()

	local fhandle, err = io.popen("ls " .. options.files[1] .. " 2>/dev/null")
	if not fhandle then
		print("ERROR: Failed to execute 'ls': " .. tostring(err))
		os.exit(1)
	end

	local files = {}
	for filename in fhandle:read("*a"):gmatch("[^\r\n]+") do
		table.insert(files, filename)
	end
	fhandle:close()

	local build_file = nil
	build_file, err = io.open("build.ninja", "w")
	if build_file == nil then
		print("Failed to create a build.ninja file.")
		os.exit(1)
	end

	local build_script = ""
	build_script = build_script .. "cflags ="
	for _, flag in ipairs(options.cflags) do
		build_script = build_script .. " -" .. flag
	end
	build_script = build_script
		.. [[


rule compile
    command = clang $cflags -c $in -o $out

rule link
    command = clang $in -o $out

]]

	for _, filename in ipairs(files) do
		build_script = build_script .. "build " .. filename:gsub("%.c$", ".o") .. ": compile " .. filename .. "\n"
	end
	build_script = build_script .. "\n"

	build_script = build_script .. "build " .. options.target .. ": link"
	for _, filename in ipairs(files) do
		build_script = build_script .. " " .. filename:gsub("%.c$", ".o")
	end
	build_script = build_script .. "\n"

	build_file:write(build_script)
	build_file:close()

	print(build_script)
end

main()
