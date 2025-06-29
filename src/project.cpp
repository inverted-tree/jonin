#include "project.hpp"
#include <stdexcept>

namespace jonin_bt {
using namespace std;

Project::Project(filesystem::path script_) : script(script_) {}

auto Project::new_Project(string script_path) -> Project {
	return Project(filesystem::path(script_path));
}

auto Project::register_Target(Target target) -> void {
	if (targets.contains(target.get_name()))
		throw runtime_error("A target with the name '" + target.get_name() +
		                    " is already defined for this project");
	targets.insert({target.get_name(), target});
}

auto Project::register_Macro(Macro macro) -> void {
	// TODO: Allow the user to overwrite the default macros
	if (macros.contains(macro.get_name()))
		throw runtime_error("A macro with the name '" + macro.get_name() +
		                    " is already defined for this project");
	macros.insert({macro.get_name(), macro});
}

// TODO: Implement a writer and call it from here
auto Project::write_build_script() -> void {}

} // namespace jonin_bt
