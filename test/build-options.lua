require("jonin")

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
