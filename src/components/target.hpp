#pragma once

#include "binding.hpp"
#include "rule.hpp"
#include "statement.hpp"
#include <vector>

namespace jonin_bt {

class Target {
  public:
	static auto new_Target() -> Target;

  private:
	std::vector<Binding> bindings;
	std::vector<Rule> rules;
	std::vector<Statement> statements;

	Target();
};

} // namespace jonin_bt
