return {
	isInstanceOf = function(value, metatable, msg)
		if type(value) ~= "table" or getmetatable(value) ~= metatable then
			if msg then
				error(msg, 2)
			else
				error("Table has wrong metatable.")
			end
		end
	end,

	isNonEmptyString = function(str, msg)
		if type(str) ~= "string" or str:len() == 0 then
			if msg then
				error(msg, 2)
			else
				error("Value must not be an empty string.")
			end
		end
	end,

	isNotNil = function(value, msg)
		if value == nil then
			if msg then
				error(msg, 2)
			else
				error("Value must not be nil.", 2)
			end
		end
	end,
}
