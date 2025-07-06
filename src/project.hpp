#pragma once

#include "components/macro.hpp"
#include "components/target.hpp"
#include <filesystem>
#include <memory>
#include <mutex>
#include <unordered_map>

namespace jonin_bt {
using TargetMap = std::unordered_map<std::string, Target>;
using MacroMap = std::unordered_map<std::string, Macro>;

class Project {
  public:
	static auto instance() -> Project *;

	auto register_Target(Target const &target) -> void;
	auto register_Macro(Macro const &macro) -> void;
	auto write_to_disk() const -> void;

	Project(Project const &obj) = delete;

  private:
	static Project *instance_;
	static std::mutex mtx;

	std::string name;
	std::filesystem::path lua_script;
	std::filesystem::path build_file;
	TargetMap targets;
	MacroMap macros;

	Project();
};

} // namespace jonin_bt
