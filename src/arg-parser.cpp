#include "arg-parser.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <vector>

namespace jonin_bt {
using namespace std;

auto parse_arguments(int const argc, char *const argv[]) -> Options {
	Options opts;
	vector<string> args(argv, argv + argc);

	if (args.size() > 2)
		throw invalid_argument("Jonin expects at most one argument");

	if (args.size() == 2 && (access(args[1].c_str(), F_OK) != -1))
		opts.script_path = args[1];

	return opts;
}

auto usage() -> void { cout << "Usage: jonin [build.lua]" << endl; }

} // namespace jonin_bt
