#include "macro.hpp"
#include <cstdlib>
#include <expected>
#include <iostream>
#include <stdexcept>

namespace jonin_bt {
using namespace std;

extern "C" {
auto build() -> int;
auto clean() -> int;
auto run() -> int;
}

Macro::Macro(string const &name_, int (*function_)(),
             string const &description_)
    : name(name_), function(function_), description(description_) {
	if (name.empty())
		throw invalid_argument("A macro name must not be empty");
}

auto Macro::new_Macro(std::string const &name, int (*function)(),
                      std::string const &description)
    -> std::expected<Macro, std::string> {
	try {
		return Macro(name, function, description);
	} catch (invalid_argument const &e) {
		return unexpected(string(e.what()));
	}
}

auto default_macro_build() -> Macro {
	auto const macro = Macro::new_Macro("build", &build, "Build the project");
	if (!macro) {
		cerr << "Failed to instanciate a default macro" << endl;
		exit(EXIT_FAILURE);
	} else
		return macro.value();
}

auto default_macro_clean() -> Macro {
	auto const macro =
	    Macro::new_Macro("clean", &clean, "Clean up the build files");
	if (!macro) {
		cerr << "Failed to instanciate a default macro" << endl;
		exit(EXIT_FAILURE);
	} else
		return macro.value();
}
auto default_macro_run() -> Macro {
	auto const macro =
	    Macro::new_Macro("run", &run, "Build and run the project");
	if (!macro) {
		cerr << "Failed to instanciate a default macro" << endl;
		exit(EXIT_FAILURE);
	} else
		return macro.value();
}

extern "C" {
auto build() -> int { return system("ninja"); }

auto clean() -> int { return system("ninja -t clean"); }

auto run() -> int {
	return system("ninja && find ./build -type f -exec {} \\;");
}
}

} // namespace jonin_bt
