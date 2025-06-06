#include "rule.hpp"
#include <expected>
#include <sstream>
#include <stdexcept>

namespace jonin_bt {
using namespace std;

static vector<string> allowed_refs = {
    "${depfile}",     "${deps}",       "${msvc_deps_prefix}",
    "${description}", "${dyndep}",     "${generator}",
    "${in}",          "${in_newline}", "${out}",
    "${restat}",      "${rspfile}",    "${rspfile_content}",
};

Rule::Rule(string const &name_, string const &command_,
           vector<Binding> const &bindings_)
    : name(name_), command(command_), bindings(bindings_) {
	if (name.empty())
		throw new invalid_argument("Rule name must not be empty");
	if (command.empty())
		throw new invalid_argument("Rule command must not be empty");
}

auto Rule::new_Rule(string const &name, string const &command)
    -> expected<Rule, string> {
	vector<Binding> no_bindings;
	return Rule::new_Rule(name, command, no_bindings);
}

auto Rule::new_Rule(string const &name, string const &command,
                    vector<Binding> const &bindings) -> expected<Rule, string> {
	try {
		return Rule(name, command, bindings);
	} catch (invalid_argument const &e) {
		return unexpected(string(e.what()));
	}
}

auto Rule::to_string() const -> string {
	ostringstream oss;

	oss << "rule " << name << "\n";
	oss << "    command = " << command << "\n";

	for (Binding const &binding : bindings)
		if (find(allowed_refs.begin(), allowed_refs.end(),
		         binding.to_referece()) != allowed_refs.end())
			oss << "    " << binding.to_string();
		else
			throw invalid_argument("Rule binding not allowed");

	return oss.str();
}

auto Rule::to_referece() const -> string { return name; }

} // namespace jonin_bt
