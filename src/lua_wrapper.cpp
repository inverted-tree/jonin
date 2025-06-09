#include "lua_wrapper.hpp"
#include "components/binding.hpp"
#include "components/rule.hpp"
#include "components/statement.hpp"
#include <iostream>
#include <stdexcept>

namespace jonin_bt {
using namespace std;

extern "C" {
auto make_target(lua_State *L) -> int;
}

LuaInstance::LuaInstance() : m_L{luaL_newstate()} { luaL_openlibs(m_L); }

LuaInstance::~LuaInstance() { lua_close(m_L); }

auto LuaInstance::exec_file(Options const opts) -> void {
	lua_register(m_L, "Target", make_target);

	auto result = luaL_loadfile(m_L, opts.script_path.c_str());
	if (result != 0) {
		cout << lua_tostring(m_L, -1) << endl;
		lua_pop(m_L, 1);

		throw runtime_error(string("Failed to load file '") + opts.script_path +
		                    string("'"));
	}

	lua_pcall(m_L, 0, LUA_MULTRET, 0);
	return;
}

extern "C" {
auto make_target(lua_State *L) -> int {
	printf("This is a C++ function called from lua!\n");
	if (!lua_istable(L, 1)) {
		return luaL_error(L, "Expected table as first argument");
	}

	lua_getfield(L, 1, "name");
	if (lua_isstring(L, -1)) {
		char const *val = lua_tostring(L, -1);
		cout << "name = " << val << endl;
	}
	lua_pop(L, 1);

	lua_getfield(L, 1, "language");
	if (lua_isstring(L, -1)) {
		char const *val = lua_tostring(L, -1);
		cout << "language = " << val << endl;
	}
	lua_pop(L, 1);

	lua_getfield(L, 1, "compiler");
	if (lua_isstring(L, -1)) {
		char const *val = lua_tostring(L, -1);
		cout << "compiler = " << val << endl;
	}
	lua_pop(L, 1);

	lua_getfield(L, 1, "cflags");
	if (lua_isstring(L, -1)) {
		char const *val = lua_tostring(L, -1);
		cout << "cflags = " << val << endl;
	}
	lua_pop(L, 1);

	lua_getfield(L, 1, "files");
	if (lua_isstring(L, -1)) {
		char const *val = lua_tostring(L, -1);
		cout << "files = " << val << endl;
	}
	lua_pop(L, 1);

	return 0;
}
}

} // namespace jonin_bt
