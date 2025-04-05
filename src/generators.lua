Binding = {}
Binding.__index = Binding

function Binding.new(key, values)
	if type(key) ~= "string" or key:len() == 0 then
		error("Binding key has to be a non-empty string.", 2)
	end
	for _, value in ipairs(values) do
		if type(value) ~= "string" then
			error("Value arguments to a binding must be strings.", 2)
		end
	end

	local binding = setmetatable({}, Binding)
	binding.key = key
	binding.values = values

	return binding
end

function Binding:toString()
	return self.key .. " = " .. table.concat(self.values, " ") .. "\n"
end

Binding.__tostring = Binding.toString

function Binding:reference()
	return "${" + self.key + "}"
end

Rule = {}
Rule.__index = Rule

function Rule.new(name, command, ...)
	if type(name) ~= "string" then
		error("Rule name has to be a string.", 2)
	end
	if name:len() == 0 then
		error("Rule name cannot be an empty string.", 2)
	end
	if type(command) ~= "string" then
		error("Rule command must be a string.", 2)
	end
	local args = { ... }
	for _, value in ipairs(args) do
		if type(value) ~= "table" or getmetatable(value) ~= Binding then
			error("Additional rule arguments must be bindings.", 2)
		end
	end

	local rule = setmetatable({}, Rule)

	rule.name = name
	rule.command = command

	for _, binding in ipairs(args) do
		rule.bindings.insert(binding)
	end

	return rule
end

function Rule:toString()
	local rule = "rule " .. self.name .. "\n"
	rule = rule .. "    command = " .. self.command .. "\n"

	for _, binding in ipairs(self.bindings or {}) do
		rule = rule .. "    " .. tostring(binding)
	end

	return rule
end

Rule.__tostring = Rule.toString

function Rule.reference(self)
	return self.name
end

BuildStatement = {}
BuildStatement.__index = BuildStatement

function BuildStatement.new(rule, inp, out)
	local statement = setmetatable({}, BuildStatement)

	if type(rule) ~= "table" or getmetatable(rule) ~= Rule then
		error("Rule argument must be of type 'Rule'.", 2)
	end
	statement.rule = rule

	for _, i in ipairs(inp) do
		if type(i) ~= "string" or i:len() == 0 then
			error("Build statement inputs have to be non-empty strings.", 2)
		end
	end
	statement.inp = inp

	for _, o in ipairs(out) do
		if type(o) ~= "string" or o:len() == 0 then
			error("Build statement outputs have to be non-empty strings.", 2)
		end
	end
	statement.out = out

	return statement
end

function BuildStatement:toString()
	local stmt = "build "
	stmt = stmt .. self.out

	stmt = stmt .. ": " .. self.rule.name .. " "
	stmt = stmt .. self.inp

	return stmt .. "\n"
end

BuildStatement.__tostring = BuildStatement.toString
