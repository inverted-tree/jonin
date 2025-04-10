-- The generators module offers a straight-forward interface to define the components that make up the Ninja build file

-- Bindings are simple key-value pairs

Binding = {}
Binding.__index = Binding

local function assertIsNonEmptyString(str, name)
	if type(str) ~= "string" or str:len() == 0 then
		local msg = nil
		if name then
			msg = "'" .. name .. "'"
		else
			msg = "Parameter"
		end
		error(msg .. " must be a non-empty string.", 3)
	end
end

function Binding.new(key, values)
	assertIsNonEmptyString(key, "Binding key")
	for _, value in ipairs(values) do
		assertIsNonEmptyString(value, "Value argument")
	end

	local binding = setmetatable({}, Binding)
	binding.key = key
	binding.values = values

	return binding
end

function Binding:toString()
	return self.key .. " = " .. table.concat(self.values, " ") .. "\n"
end

function Binding:reference()
	return "${" + self.key + "}"
end

Binding.__tostring = Binding.toString

-- Rules define templates for defined actions like code compile

Rule = {}
Rule.__index = Rule

local function assertIsInstanceOf(value, metatable, errmsg)
	if type(value) ~= "table" or getmetatable(value) ~= metatable then
		error(errmsg, 3)
	end
end

function Rule.new(name, command, ...)
	assertIsNonEmptyString(name, "Rule name")
	assertIsNonEmptyString(command, "Command")
	for _, value in ipairs({ ... }) do
		assertIsInstanceOf(value, Binding, "Additional rule arguments must be bindings.")
	end

	local rule = setmetatable({}, Rule)

	rule.name = name
	rule.command = command

	for _, binding in ipairs({ ... }) do
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

function Rule.reference(self)
	return self.name
end

Rule.__tostring = Rule.toString

-- Build statements provide parameters to rules and execute them

BuildStatement = {}
BuildStatement.__index = BuildStatement

function BuildStatement.new(rule, inp, out)
	assertIsInstanceOf(rule, Rule, "Build statement argument must be a rule.")
	for _, i in ipairs(inp) do
		assertIsNonEmptyString(i, "Build statement input")
	end
	for _, o in ipairs(out) do
		assertIsNonEmptyString(o, "Build statement output")
	end

	local statement = setmetatable({}, BuildStatement)
	statement.rule = rule
	statement.inp = inp
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
