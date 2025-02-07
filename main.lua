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
end

main()
