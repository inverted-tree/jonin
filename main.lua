-- This will become a high-level build system for C projects

local function main()
	print("Hello world from the jougetsu build system!")

	local handle, err = io.popen("ls")
	if not handle then
		print("ERROR: Failed to execute 'ls': " .. tostring(err))
		os.exit(1)
	end

	local result = handle:read("*a")
	handle:close()

	if result then
		print("The current directory content is:")
		print(result)
	else
		print("ERROR: No output recieved from 'ls'.")
	end

	local build_file = io.open("build.ninja", "w")
	if build_file == nil then
		os.exit(1)
	end
	local config = [[
cflags = -Wall

rule cc
    command = clang $cflags -c $in -o $out

rule ln
    command = clang $in -o $out

build main.o: cc main.c
build foo.o: cc foo.c
build bar.o: cc bar.c

build main: ln main.o foo.o bar.o
]]

	build_file:write(config)
end

main()
