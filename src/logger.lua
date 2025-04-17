local function log(mode, msg)
	local string = nil
	if mode == "error" then
		string = "\27[31;1m[Error]\27[0m "
	elseif mode == "info" then
		string = ""
	end
	print(string .. msg)
end

return {
	log = log,
}
