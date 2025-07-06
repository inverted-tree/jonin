Target("jonin", {
	compiler = "clang++",
	cxxflags = "-Wall -Wextra -Werror -I/opt/homebrew/include/lua5.4 -std=c++23",
	ldflags = "-L/opt/homebrew/lib -llua -lm",
	files = { "src/*.cpp", "src/**/*.cpp" },
}, "The main target of the build system.")

Macro("TestMacro", function()
	print("Hello from a test macro!")
end, "Just a test macro!")
