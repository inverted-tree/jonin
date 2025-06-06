#include "binding.hpp"
#include <expected>
#include <sstream>
#include <stdexcept>

namespace jonin_bt {
using namespace std;

Binding::Binding(const string &name_, vector<string> const &values_)
    : name(name_), values(values_) {
	if (name.empty())
		throw invalid_argument("A bindings name cannot be empty");
};

auto Binding::new_Binding(const std::string &name,
                          const std::vector<std::string> &values)
    -> expected<Binding, string> {

	try {
		return Binding(name, values);
	} catch (invalid_argument const &e) {
		return unexpected(string(e.what()));
	}
}

auto Binding::to_string() const -> string {
	ostringstream oss;
	oss << name << " =";

	for (auto const &value : values)
		oss << " " << value;

	oss << "\n";
	return oss.str();
}

auto Binding::to_reference() const -> string {
	ostringstream oss;
	oss << "${" << name << "}";
	return oss.str();
}

auto Binding::get_name() const -> std::string { return name; }

} // namespace jonin_bt
