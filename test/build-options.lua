require("include.jonin")

Project("ExampleTestProject")

Version(0.1)

Target("HelloWorld", {
	cc = "clang",
	cflags = {
		"-Wall",
		"-Wextra",
	},
	files = {
		"./test/hello-world/src/",
	},
})
