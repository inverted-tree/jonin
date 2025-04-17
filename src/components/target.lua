local asserts = require("src.assertions")
require("src.components.rule")

-- Targets provide a representation of the defined build targets

BuildTarget = {}
BuildTarget.__index = BuildTarget

function BuildTarget.new(name, rules, statements)
	asserts.isNonEmptyString(name, "Target name cannot be an empty string.")
	for _, rule in pairs(rules) do
		asserts.isInstanceOf(rule, Rule, "Elements of target parameter 'rules' must all have the 'Rule' metatable.")
	end
	for _, stmt in ipairs(statements) do
		asserts.isInstanceOf(
			stmt,
			BuildStatement,
			"Elements of target parameter 'statements' must all have the 'BuildStatement' metatable."
		)
	end
	local target = setmetatable({}, BuildTarget)

	target.name = name
	target.rules = rules
	target.statements = statements

	return target
end
