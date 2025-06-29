print("This lua script gets executed")

Target({
	name = "TestTarget",
	language = "c",
	compiler = "clang",
	cflags = "-Wall",
	files = "test/**/src",
})
