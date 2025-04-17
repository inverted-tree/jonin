local asserts = require("src.assertions")

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
	rule.bindings = {}

	for _, binding in ipairs({ ... }) do
		table.insert(rule.bindings, binding)
	end

	return rule
end

local allowedKeys = {
	"depfile",
	"deps",
	"msvc_deps_prefix",
	"description",
	"dyndep",
	"generator",
	"in",
	"in_newline",
	"out",
	"restat",
	"rspfile",
	"rspfile_content",
}

local utils = require("src.utils")
function Rule:toString()
	local rule = "rule " .. self.name .. "\n"
	rule = rule .. "    command = " .. self.command .. "\n"

	for _, binding in ipairs(self.bindings) do
		if utils.contains(allowedKeys, binding.key) then
			rule = rule .. "    " .. tostring(binding)
		end
	end

	return rule
end

function Rule.reference(self)
	return self.name
end

Rule.__tostring = Rule.toString
