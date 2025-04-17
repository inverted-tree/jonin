local asserts = require("src.assertions")

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
	return self.key .. " = " .. self.values .. "\n"
end

function Binding:reference()
	return "${" + self.key + "}"
end

Binding.__tostring = Binding.toString
