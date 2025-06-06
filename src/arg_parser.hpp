#pragma once

#include <string>

namespace jonin_bt {

struct Options {
	std::string script_path;
};

auto parse_arguments(int const argc, char *const argv[]) -> Options;

auto usage() -> void;

} // namespace jonin_bt
