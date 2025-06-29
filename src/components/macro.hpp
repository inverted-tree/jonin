#pragma once

#include <expected>
#include <string>

namespace jonin_bt {
class Macro {
  public:
	static auto new_Macro(std::string const &name, int (*function)(),
	                      std::string const &description)
	    -> std::expected<Macro, std::string>;

	auto execute() -> void;

  private:
	std::string name;
	int (*function)();
	std::string description;

	Macro(std::string const &name, int (*function)(),
	      std::string const &description);
};

auto default_macro_build() -> Macro;
auto default_macro_clean() -> Macro;
auto default_macro_run() -> Macro;

} // namespace jonin_bt
