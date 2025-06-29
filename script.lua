print("This lua script gets executed")

Target("TestTarget", {
	language = "c",
	compiler = "clang",
	cflags = "-Wall",
	files = { "test/main.c" },
}, "A basic target to thest if the parser works")
