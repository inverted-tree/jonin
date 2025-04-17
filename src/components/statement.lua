local asserts = require("src.assertions")

-- Build statements provide parameters to rules and execute them

BuildStatement = {}
BuildStatement.__index = BuildStatement

function BuildStatement.new(rule, inp, out)
	asserts.isInstanceOf(rule, Rule, "Build statement argument must be a rule.")
	for _, i in ipairs(inp) do
		asserts.isNonEmptyString(i, "Build statement input")
	end
	for _, o in ipairs(out) do
		asserts.isNonEmptyString(o, "Build statement output")
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
	stmt = stmt .. self.inp .. "\n"

	return stmt
end

BuildStatement.__tostring = BuildStatement.toString
