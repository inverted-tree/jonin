print("This lua script gets executed")

Target("TestTarget", {
	language = "c",
	compiler = "clang",
	cflags = "-Wall",
	files = { "test/main.c" },
}, "A basic target to test if the parser works")

Macro("TestMacro", function()
	print("Hello from a test macro!")
end, "Just a test macro!")
