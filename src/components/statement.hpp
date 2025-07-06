#pragma once

#include "binding.hpp"
#include "rule.hpp"
#include <expected>
#include <string>
#include <vector>

namespace jonin_bt {

class Statement {
  public:
	static auto new_Statement(Rule const &rule,
	                          std::vector<std::string> const &inputs,
	                          std::vector<std::string> const &outputs)
	    -> std::expected<Statement, std::string>;

	static auto new_Statement(Rule const &rule,
	                          std::vector<std::string> const &inputs,
	                          std::vector<std::string> const &outputs,
	                          std::vector<Binding> const &bindings)
	    -> std::expected<Statement, std::string>;

	auto get_inputs() const -> std::vector<std::string>;
	auto get_outputs() const -> std::vector<std::string>;
	auto to_string() const -> std::string;

  private:
	Rule rule;
	std::vector<std::string> inputs;
	std::vector<std::string> outputs;
	std::vector<Binding> bindings;

	Statement(Rule const &rule_, std::vector<std::string> const &inputs_,
	          std::vector<std::string> const &outputs_,
	          std::vector<Binding> const &bindings_);
};

} // namespace jonin_bt
