#include "arg-parser.hpp"
#include "lua-wrapper.hpp"
#include "lua.hpp"
#include "project.hpp"
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <stdexcept>

using namespace jonin_bt;

int main(int argc, char **argv) {
	Options opts;
	try {
		opts = parse_arguments(argc, argv);
	} catch (std::invalid_argument e) {
		usage();
		return EXIT_FAILURE;
	}

	if (opts.script_path.empty())
		opts.script_path = "script.lua";
	Project project = Project::new_Project(opts.script_path);

	try {
		LuaInstance Lua;
		Lua.exec_file(opts);
	} catch (std::runtime_error e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
