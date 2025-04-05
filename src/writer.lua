local function writeFile(config)
	local file = io.open(config.filePath, "w")
	if file == nil then
		error(string.format("Cannot open file '%s'", config.filePath))
	end

	file:write(tostring(config.flags))
	file:write("\n")
	file:write(tostring(config.compileRule))
	file:write(tostring(config.linkRule))
	file:write("\n")
	for _, stmt in ipairs(config.compileStmts) do
		file:write(tostring(stmt))
	end
	file:write("\n")
	for _, stmt in ipairs(config.linkStmts) do
		file:write(tostring(stmt))
	end
	file:write("\n")

	file:flush()
	file:close()
end

return {
	writeFile = writeFile,
}
