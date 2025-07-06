#pragma once

#include "rule.hpp"
#include "statement.hpp"
#include <expected>
#include <filesystem>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace jonin_bt {

class Target {
  public:
	static auto
	new_Target(std::string name,
	           std::unordered_map<std::string, std::optional<std::string>>
	               target_options,
	           std::optional<std::string> description)
	    -> std::expected<Target, std::string>;

	auto get_name() const -> std::string;
	auto to_string() const -> std::string;

  private:
	std::string name;
	std::string compiler;
	std::unordered_map<std::string, std::optional<std::vector<std::string>>>
	    flags;
	std::vector<std::filesystem::path> source_files;
	std::optional<std::string> description;

	auto get_bindings() const -> std::vector<Binding>;
	auto get_rules() const -> std::vector<Rule>;
	auto get_statements() const -> std::vector<Statement>;

	auto build_compile_command() const -> std::string;
	auto build_link_command() const -> std::string;

	Target(
	    std::string name, std::string compiler,
	    std::unordered_map<std::string, std::optional<std::vector<std::string>>>
	        flags,
	    std::vector<std::filesystem::path> source_files,
	    std::optional<std::string> description);
};

} // namespace jonin_bt
