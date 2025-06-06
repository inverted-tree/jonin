#pragma once

#include "binding.hpp"
#include <expected>
#include <string>

namespace jonin_bt {

class Rule {
  public:
	static auto new_Rule(std::string const &name, std::string const &command)
	    -> std::expected<Rule, std::string>;
	static auto new_Rule(std::string const &name, std::string const &command,
	                     std::vector<Binding> const &bindings)
	    -> std::expected<Rule, std::string>;

	auto to_string() const -> std::string;

	auto to_reference() const -> std::string;

  private:
	std::string name;
	std::string command;
	std::vector<Binding> bindings;

	Rule(std::string const &name_, std::string const &command_,
	     std::vector<Binding> const &bindings_);
};

} // namespace jonin_bt
