-- This will become a high-level build system for C projects

local function main()
	local fhandle, err = io.popen("ls test/*.c 2>/dev/null")
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

	local config = [[
cflags = -Wall

rule compile
    command = clang $cflags -c $in -o $out

rule link
    command = clang $in -o $out

]]

	for _, filename in ipairs(files) do
		config = config .. "build " .. filename:gsub("%.c$", ".o") .. ": compile " .. filename .. "\n"
	end
	config = config .. "\n"

	config = config .. "build main: link"
	for _, filename in ipairs(files) do
		config = config .. " " .. filename:gsub("%.c$", ".o")
	end
	config = config .. "\n"

	build_file:write(config)
	build_file:close()

	print(config)
end

main()
