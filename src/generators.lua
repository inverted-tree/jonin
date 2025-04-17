-- The generators module offers a straight-forward interface to define the components that make up the Ninja build file

return {
	binding = require("src.components.binding"),
	rule = require("src.components.rule"),
	statement = require("src.components.statement"),
	target = require("src.components.target"),
}
