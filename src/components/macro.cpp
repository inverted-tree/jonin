#include "macro.hpp"
#include <cstdlib>
#include <expected>
#include <iostream>
#include <stdexcept>

extern "C" {
#include "lua.h"
#include <lauxlib.h>
}

namespace jonin_bt {
using namespace std;

extern "C" {
auto build(lua_State *L) -> int;
auto clean(lua_State *L) -> int;
auto run(lua_State *L) -> int;
}

Macro::Macro(string const &name_, lua_State *L_, int lua_ref_,
             string const &description_)
    : name(name_), L(L_), function(lua_ref_), description(description_) {
	if (name.empty())
		throw invalid_argument("A macro's name must not be empty");
}

Macro::Macro(string const &name_, int (*function_)(lua_State *),
             string const &description_)
    : name(name_), function(function_), description(description_) {
	if (name.empty())
		throw invalid_argument("A macro's name must not be empty");
}

auto Macro::print_macro() const -> void {
	cout << "Macro name: " << name << endl;
	cout << "Macro description: " << description << endl;
}

auto Macro::get_name() const -> std::string { return name; }

auto Macro::new_Lua_Macro(std::string const &name, lua_State *L,
                          int lua_function_index,
                          std::string const &description)
    -> std::expected<Macro, std::string> {
	lua_pushvalue(L, lua_function_index);
	int ref = luaL_ref(L, LUA_REGISTRYINDEX);
	try {
		return Macro(name, L, ref, description);
	} catch (invalid_argument const &e) {
		return unexpected(string(e.what()));
	}
}

auto Macro::new_C_Macro(std::string const &name, int (*function)(lua_State *),
                        std::string const &description)
    -> std::expected<Macro, std::string> {
	try {
		return Macro(name, function, description);
	} catch (invalid_argument const &e) {
		return unexpected(string(e.what()));
	}
}

auto default_macro_build() -> Macro {
	auto const macro = Macro::new_C_Macro("build", &build, "Build the project");
	if (!macro) {
		cerr << "Failed to instanciate a default macro" << endl;
		exit(EXIT_FAILURE);
	} else
		return macro.value();
}

auto default_macro_clean() -> Macro {
	auto const macro =
	    Macro::new_C_Macro("clean", &clean, "Clean up the build files");
	if (!macro) {
		cerr << "Failed to instanciate a default macro" << endl;
		exit(EXIT_FAILURE);
	} else
		return macro.value();
}
auto default_macro_run() -> Macro {
	auto const macro =
	    Macro::new_C_Macro("run", &run, "Build and run the project");
	if (!macro) {
		cerr << "Failed to instanciate a default macro" << endl;
		exit(EXIT_FAILURE);
	} else
		return macro.value();
}

extern "C" {
auto build(lua_State *L) -> int {
	(void)L;
	return system("ninja");
}

auto clean(lua_State *L) -> int {
	(void)L;
	return system("ninja -t clean");
}

auto run(lua_State *L) -> int {
	(void)L;
	return system("ninja && find ./build -type f -exec {} \\;");
}
}

} // namespace jonin_bt
