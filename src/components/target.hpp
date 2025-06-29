#pragma once

#include "binding.hpp"
#include "rule.hpp"
#include "statement.hpp"
#include <expected>
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace jonin_bt {

/**
 * Enumerating supported languages.
 */
enum Language {
	C,
	CPP,
	FORTRAN,
	UNDEFINED,
};

/**
 * Internal representation of a build target.
 */
class Target {
  public:
	static auto
	new_Target(std::map<std::string, std::optional<std::string>> target_options)
	    -> std::expected<Target, std::string>;

	auto print_tgt() -> void;

  private:
	/**
	 * Name of the target. Lets one reference specific targets individually.
	 * Required to be non-empty. Defaults to "Default" if not specified and
	 * only one target is defined.
	 */
	std::string name;
	/**
	 * Language the target source code is written in. Has impliciations for
	 * automations.
	 */
	Language language;
	/**
	 * Sets the compiler that will be used for compiling this target.
	 * Defaults to 'cc'.
	 */
	std::optional<std::string> compiler;
	/**
	 * C compiler flags. Will be 'nullopt' for other languages.
	 */
	std::optional<std::vector<std::string>> cflags;
	/**
	 * C++ compiler flags. Will be 'nullopt' for other languages.
	 */
	std::optional<std::vector<std::string>> cxxflags;
	/**
	 * Fortran compiler flags. Will be 'nullopt' for other languages.
	 */
	std::optional<std::vector<std::string>> fcflags;
	/**
	 * Generic linker flags.
	 */
	std::optional<std::vector<std::string>> ldflags;
	/**
	 * C++ preprocessor flags. Will be 'nullopt' for other languages.
	 */
	std::optional<std::vector<std::string>> cppflags;
	/**
	 * Assembly flags. Will be 'nullopt' for other languages.
	 */
	std::optional<std::vector<std::string>> asflags;
	/**
	 * Archiver flags for creating static libraries. Will be 'nullopt' for
	 * other languages.
	 */
	std::optional<std::vector<std::string>> arflags;
	/**
	 * Source files for this target.
	 */
	std::vector<std::filesystem::path> source_files;
	/**
	 * Dependencies that will be linked to the target.
	 */
	std::optional<std::vector<std::string>> dependencies;

	Target(std::string name_, Language language_,
	       std::optional<std::string> compiler_,
	       std::optional<std::vector<std::string>> cflags_,
	       std::optional<std::vector<std::string>> cxxflags_,
	       std::optional<std::vector<std::string>> fcflags_,
	       std::optional<std::vector<std::string>> ldflags_,
	       std::optional<std::vector<std::string>> cppflags_,
	       std::optional<std::vector<std::string>> asflags_,
	       std::optional<std::vector<std::string>> arflags_,
	       std::vector<std::filesystem::path> source_files_,
	       std::optional<std::vector<std::string>> dependencies_);
};

} // namespace jonin_bt
