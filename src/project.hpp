#pragma once

#include "components/macro.hpp"
#include "components/target.hpp"
#include <filesystem>
#include <unordered_map>

namespace jonin_bt {

class Project {
  public:
	static auto new_Project(std::string script_path) -> Project;
	auto register_Target(Target target) -> void;
	auto register_Macro(Macro macro) -> void;
	auto write_build_script() -> void;

  private:
	std::string name;
	std::filesystem::path script;
	std::filesystem::path build_file;
	std::unordered_map<std::string, Target> targets;
	std::unordered_map<std::string, Macro> macros;

	Project(std::filesystem::path script);
};

} // namespace jonin_bt
