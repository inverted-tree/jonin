#include "lua-wrapper.hpp"
#include "components/binding.hpp"
#include "components/macro.hpp"
#include "components/rule.hpp"
#include "components/statement.hpp"
#include "components/target.hpp"
#include "project.hpp"
#include <expected>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <unordered_map>

namespace jonin_bt {
using namespace std;

extern "C" {
auto make_target(lua_State *L) -> int;
auto make_macro(lua_State *L) -> int;
}

LuaInstance::LuaInstance() : m_L{luaL_newstate()} { luaL_openlibs(m_L); }

LuaInstance::~LuaInstance() { lua_close(m_L); }

auto LuaInstance::exec_file(Options const &opts) -> void {
	lua_register(m_L, "Target", make_target);
	lua_register(m_L, "Macro", make_macro);

	auto result = luaL_loadfile(m_L, opts.script_path.c_str());
	if (result != 0) {
		cout << lua_tostring(m_L, -1) << endl;
		lua_pop(m_L, 1); // Remove the error from the stack

		throw runtime_error(string("Failed to load file '") + opts.script_path +
		                    string("'"));
	}

	result = lua_pcall(m_L, 0, LUA_MULTRET, 0);
	if (result != 0) {
		string error = lua_tostring(m_L, -1);
		lua_pop(m_L, 1);
		throw runtime_error(
		    "The lua interpreter encountered a runtime error: " + error);
	}

	return;
}

extern "C" {
unordered_map<string, optional<string>> const _target_options = {
    {"name", nullopt},    {"language", nullopt}, {"compiler", nullopt},
    {"cflags", nullopt},  {"cxxflags", nullopt}, {"fcflags", nullopt},
    {"ldflags", nullopt}, {"cppflags", nullopt}, {"asflags", nullopt},
    {"arflags", nullopt}, {"files", nullopt},    {"dependencies", nullopt}};

auto make_target(lua_State *L) -> int {
	int nargs = lua_gettop(L);
	if (nargs < 2 || nargs > 3)
		return luaL_error(
		    L,
		    "'Target' expects 2 (+ 1 optional) arguments but %d were provided",
		    nargs);

	if (!lua_isstring(L, 1))
		return luaL_error(L, "First argument to 'Target' must be a string");
	string name = lua_tostring(L, 1);

	if (!lua_istable(L, 2)) {
		return luaL_error(L, "Second argument to 'Target' must be a table");
	}

	optional<string> description = nullopt;
	if (nargs == 3) {
		if (!lua_isstring(L, 3))
			return luaL_error(L, "Third argument to 'Target' must be a string");
		description = lua_tostring(L, 3);
	}

	unordered_map<string, optional<string>> target_options = _target_options;
	lua_pushnil(L);
	while (lua_next(L, 2) != 0) {
		// stack: key at -2, value at -1
		if (!lua_isstring(L, -2)) {
			lua_pop(L, 2);
			return luaL_error(L, "Table keys must be strings");
		}

		string key = lua_tostring(L, -2);
		auto it = target_options.find(key);
		if (it == target_options.end()) {
			lua_pop(L, 2);
			return luaL_error(L, ("Invalid field in table: " + key).c_str());
		}

		if (lua_isnil(L, -1)) {
			it->second = std::nullopt;
		} else if (key == "files") {
			if (lua_isstring(L, -1)) {
				it->second = string(lua_tostring(L, -1));
			} else if (lua_istable(L, -1)) {
				// Concat all strings into a space-separated list
				string result;
				lua_Integer table_length = lua_rawlen(L, -1);
				for (lua_Integer i = 1; i <= table_length; i++) {
					lua_rawgeti(L, -1, i);
					if (!lua_isstring(L, -1)) {
						lua_pop(L, 2); // value + parent table
						return luaL_error(
						    L, "The 'files' list must contain only strings");
					}
					if (!result.empty())
						result += " ";
					result += lua_tostring(L, -1);
					lua_pop(L, 1);
				}
				it->second = result;
			} else {
				lua_pop(L, 2);
				return luaL_error(
				    L,
				    "'files' must be a string or a table of strings (or nil)");
			}
		} else if (lua_isstring(L, -1)) {
			it->second = string(lua_tostring(L, -1));

		} else {
			lua_pop(L, 2);
			return luaL_error(
			    L, ("Value for field '" + key + "' must be a string or nil")
			           .c_str());
		}

		lua_pop(L, 1);
	}

	auto target = Target::new_Target(name, target_options, description);
	if (target.has_value()) {
		Project::instance()->register_Target(target.value());
		return 0;
	} else
		return luaL_error(L, "Failed to create target");
}

auto make_macro(lua_State *L) -> int {
	if (!lua_isstring(L, 1))
		return luaL_error(L, "First argument to Macro must be a string");
	if (!lua_isfunction(L, 2))
		return luaL_error(L, "Second argument to Macro must be a function");

	// stack: name, function, (description)
	char const *name = lua_tostring(L, 1);

	char const *description = "";
	if (lua_gettop(L) >= 3 && lua_isstring(L, 3)) {
		description = lua_tostring(L, 3);
	}

	int lua_function_index = 2;
	auto macro = Macro::new_Lua_Macro(name, L, lua_function_index, description);
	if (!macro) {
		return luaL_error(L, "Failed to regiser macro '%s'", name);
	}
	Project::instance()->register_Macro(macro.value());

	lua_getglobal(L, "macros");

	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		lua_newtable(L);
		lua_setglobal(L, "macros");
		lua_getglobal(L, "macros");
	}

	lua_pushvalue(L, 2);       // Copy the function to the top of the stack
	lua_setfield(L, -2, name); // macros[name] = function
	lua_pop(L, 1);             // Pop the macros table

	return 0;
}
}

} // namespace jonin_bt
