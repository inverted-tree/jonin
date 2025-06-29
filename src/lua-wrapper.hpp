#pragma once

#include "arg-parser.hpp"
#include "lua.hpp"

namespace jonin_bt {

class LuaInstance {
  public:
	LuaInstance();
	~LuaInstance();

	auto exec_file(Options const &opts) -> void;

  private:
	lua_State *const m_L;
};

} // namespace jonin_bt
