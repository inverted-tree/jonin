#pragma once

#include <expected>
#include <string>
#include <vector>

namespace jonin_bt {

/**
 * Represents a Ninja-binding such as 'cflags = -Wall'
 */
class Binding {
  public:
	static auto new_Binding(std::string const &name,
	                        std::vector<std::string> const &values)
	    -> std::expected<Binding, std::string>;

	auto to_string() const -> std::string;
	auto to_reference() const -> std::string;
	auto get_name() const -> std::string;

  private:
	std::string name;
	std::vector<std::string> values;

	Binding(std::string const &name_, std::vector<std::string> const &values_);
};

} // namespace jonin_bt
