#include "statement.hpp"
#include "binding.hpp"
#include <expected>
#include <sstream>
#include <stdexcept>

namespace jonin_bt {
using namespace std;

Statement::Statement(Rule const &rule_, vector<string> const &inputs_,
                     vector<string> const &outputs_,
                     vector<Binding> const &bindings_)
    : rule(rule_), inputs(inputs_), outputs(outputs_), bindings(bindings_) {}

auto Statement::new_Statement(Rule const &rule, vector<string> const &inputs,
                              vector<string> const &outputs,
                              vector<Binding> const &bindings)
    -> expected<Statement, string> {
	try {
		return Statement(rule, inputs, outputs, bindings);
	} catch (invalid_argument const &e) {
		return unexpected(string(e.what()));
	}
}

auto Statement::new_Statement(Rule const &rule, vector<string> const &inputs,
                              vector<string> const &outputs)
    -> expected<Statement, string> {
	vector<Binding> no_bindings;
	return new_Statement(rule, inputs, outputs, no_bindings);
}

auto Statement::get_inputs() const -> std::vector<std::string> {
	return inputs;
}

auto Statement::get_outputs() const -> std::vector<std::string> {
	return outputs;
}

auto Statement::to_string() const -> string {
	ostringstream oss;
	oss << "build";
	for (auto const &output : outputs)
		oss << " " << output;
	oss << ": " << rule.to_reference();
	for (auto const &input : inputs)
		oss << " " << input;
	oss << endl;

	for (auto const &binding : bindings)
		oss << "    " << binding.to_string();

	return oss.str();
}

} // namespace jonin_bt
