#pragma once

#include <expected>
#include <string>
#include <variant>

extern "C" {
#include "lua.h"
}

namespace jonin_bt {
using MacroFunction = std::variant<int, int (*)(lua_State *)>;

class Macro {
  public:
	static auto new_C_Macro(std::string const &name,
	                        int (*function)(lua_State *),
	                        std::string const &description)
	    -> std::expected<Macro, std::string>;
	static auto new_Lua_Macro(std::string const &name, lua_State *L,
	                          int lua_function_index,
	                          std::string const &description)
	    -> std::expected<Macro, std::string>;

	auto execute() const -> void;
	auto print_macro() const -> void;
	auto get_name() const -> std::string;

  private:
	std::string name;
	lua_State *L = nullptr;
	MacroFunction function;
	std::string description;

	Macro(std::string const &name, lua_State *L, int lua_ref,
	      std::string const &description);
	Macro(std::string const &name, int (*function)(lua_State *),
	      std::string const &description);
};

auto default_macro_build() -> Macro;
auto default_macro_clean() -> Macro;
auto default_macro_run() -> Macro;

} // namespace jonin_bt
