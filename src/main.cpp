#include "arg_parser.hpp"
#include "lua.hpp"
#include "lua_wrapper.hpp"
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

	try {
		LuaInstance L;
		L.exec_file(opts);
	} catch (std::runtime_error e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
