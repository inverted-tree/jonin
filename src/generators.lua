-- The generators module offers a straight-forward interface to define the components that make up the Ninja build file

local utils = require("src.utils")
local asserts = utils.prequire("src.assertions")

-- Bindings are simple key-value pairs

Binding = {}
Binding.__index = Binding

function Binding.new(key, values)
	asserts.isNonEmptyString(key, "Binding key must not be an empty string.")
	for _, value in ipairs(values) do
		asserts.isNonEmptyString(value, "Value argument must not be an empty string.")
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

function Rule.new(name, command, ...)
	asserts.isNonEmptyString(name, "Rule name")
	asserts.isNonEmptyString(command, "Command")
	for _, value in ipairs({ ... }) do
		asserts.isInstanceOf(value, Binding, "Additional rule arguments must be bindings.")
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
	asserts.isInstanceOf(rule, Rule, "Build statement argument must be a rule.")
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
