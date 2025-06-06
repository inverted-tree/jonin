#include "lua_wrapper.hpp"
#include "components/binding.hpp"
#include "components/rule.hpp"
#include "components/statement.hpp"
#include <iostream>
#include <stdexcept>

namespace jonin_bt {
using namespace std;

LuaInstance::LuaInstance() : m_L{luaL_newstate()} { luaL_openlibs(m_L); }

LuaInstance::~LuaInstance() { lua_close(m_L); }

auto LuaInstance::exec_file(Options const opts) -> void {
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

} // namespace jonin_bt
