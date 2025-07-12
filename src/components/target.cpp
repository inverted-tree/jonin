#include "target.hpp"
#include "binding.hpp"
#include "rule.hpp"
#include "statement.hpp"
#include <algorithm>
#include <cstring>
#include <expected>
#include <filesystem>
#include <glob.h>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <unordered_set>

namespace jonin_bt {
using namespace std;

auto split_optional_string(optional<string> &str) -> optional<vector<string>>;
auto expand_path(string &glob) -> vector<filesystem::path>;

Target::Target(
    std::string name_, std::string compiler_,
    std::unordered_map<std::string, std::optional<std::vector<std::string>>>
        flags_,
    std::vector<std::filesystem::path> source_files_,
    std::optional<std::string> description_)
    : name(name_), compiler(compiler_), flags(flags_),
      source_files(source_files_), description(description_) {
	if (name.empty())
		throw invalid_argument("A targets name must not be empty");
	for (filesystem::path file : source_files)
		if (!filesystem::exists(file))
			throw invalid_argument("Source file '" + string(file) +
			                       "' does not exist");
}

auto Target::get_name() const -> string { return name; }

auto Target::get_bindings() const -> vector<Binding> {
	vector<Binding> bindings;
	for (auto flag : flags) {
		auto name = flag.first;

		auto values = flag.second.value_or(vector<string>{
		    "",
		});

		auto b = Binding::new_Binding(name, values);
		if (b.has_value())
			bindings.push_back(b.value());
		else
			throw runtime_error("Failed to create binding");
	}

	return bindings;
}

auto Target::build_compile_command() const -> string {
	ostringstream cmd;

	cmd << compiler;

	unordered_set<string> allowed_bindings = {
	    "cflags", "cxxflags", "fcflags", "cppflags", "asflags",
	};
	auto bindings = get_bindings();
	for (auto b : bindings)
		if (allowed_bindings.contains(b.get_name()))
			cmd << " " << b.to_reference();

	cmd << " -c $in -o $out";

	return cmd.str();
}

auto Target::build_link_command() const -> string {
	ostringstream cmd;
	cmd << compiler;

	unordered_set<string> allowed_bindings = {
	    "ldflags",
	    "arflags",
	};
	auto bindings = get_bindings();
	for (auto b : bindings)
		if (allowed_bindings.contains(b.get_name()))
			cmd << " " << b.to_reference();

	cmd << " $in -o $out";

	return cmd.str();
}

auto Target::get_rules() const -> vector<Rule> {
	vector<Rule> rules;
	// TODO: Generate the required rules (build, link)

	auto register_rule = [&rules](expected<Rule, string> rule) {
		if (rule.has_value())
			rules.push_back(rule.value());
		else
			throw runtime_error("Failed to create rule");
	};

	register_rule(Rule::new_Rule({"compile_" + name}, build_compile_command()));
	register_rule(Rule::new_Rule({"link_" + name}, build_link_command()));

	return rules;
}

auto Target::get_statements() const -> vector<Statement> {
	vector<Statement> statements;

	auto register_statement = [&statements](expected<Statement, string> stmt) {
		if (stmt.has_value())
			statements.push_back(stmt.value());
		else
			throw runtime_error("Failed to create statement");
	};

	auto rules = Target::get_rules();

	auto switch_to_build_dir = [](const filesystem::path &path) {
		filesystem::path relative;
		if (path.has_root_path())
			relative = path.lexically_relative(path.root_path());
		else
			relative = path;

		return filesystem::path("build") / relative;
	};

	for (auto file : source_files) {
		register_statement(Statement::new_Statement(
		    rules[0], vector<string>{file},
		    vector<string>{switch_to_build_dir(file.replace_extension(".o"))}));
	}

	vector<string> targets;
	for (Statement &stmt : statements) {
		for (const auto &output : stmt.get_outputs()) {
			targets.push_back(output);
		}
	}

	register_statement(
	    Statement::new_Statement(rules[1], targets, vector<string>{name}));

	return statements;
}

auto Target::to_string() const -> string {
	auto bindings = get_bindings();
	auto rules = get_rules();
	auto statements = get_statements();

	stringstream ss;

	for (auto b : bindings)
		ss << b.to_string();

	if (!bindings.empty())
		ss << endl;

	for (auto r : rules)
		ss << r.to_string();
	ss << endl;

	for (auto s : statements)
		ss << s.to_string();

	return ss.str();
}

auto strview_tolower(string_view sv) -> string {
	string result(sv);
	transform(result.begin(), result.end(), result.begin(),
	          [](unsigned char c) { return tolower(c); });
	return result;
}

auto Target::new_Target(string name,
                        unordered_map<string, optional<string>> target_options,
                        optional<string> description)
    -> expected<Target, string> {
	std::replace(name.begin(), name.end(), ' ', '_');

	string compiler;
	if (target_options.contains("compiler")) {
		auto maybe_c = target_options["compiler"];
		if (maybe_c.has_value())
			compiler = maybe_c.value();
	}

	auto splitstr = [](optional<string> &str) -> optional<vector<string>> {
		if (!str.has_value())
			return nullopt;

		istringstream ss(*str);
		vector<string> result;
		string word;

		while (ss >> word)
			result.push_back(word);

		return result.empty() ? nullopt : optional(result);
	};

	unordered_map<string, optional<vector<string>>> flags;
	unordered_set<string> supported_flags = {
	    "cflags",   "cxxflags", "fcflags", "ldflags",
	    "cppflags", "asflags",  "arflags",
	};

	for (auto option : target_options) {
		if (supported_flags.contains(option.first))
			if (option.second.has_value())
				flags.insert({option.first, splitstr(option.second)});
	}

	vector<filesystem::path> source_files;
	if (target_options.contains("files")) {
		auto maybe_files = target_options["files"];
		if (maybe_files.has_value()) {
			auto maybe_split_files = splitstr(maybe_files);
			if (maybe_split_files.has_value()) {
				auto split_files = maybe_split_files.value();
				for (auto &file_glob : split_files) {
					auto expanded_paths = expand_path(file_glob);
					source_files.insert(source_files.end(),
					                    expanded_paths.begin(),
					                    expanded_paths.end());
				}
			}
		}
	} else {
		// TODO: Automatically search for source files
	}

	try {
		Target target =
		    Target(name, compiler, flags, source_files, description);
		return target;
	} catch (invalid_argument const &e) {
		return unexpected(string(e.what()));
	}
}

auto expand_path(string &glob_pattern) -> vector<filesystem::path> {
	glob_t glob_result;
	memset(&glob_result, 0, sizeof(glob_result));

	int return_value =
	    glob(glob_pattern.c_str(), GLOB_TILDE, NULL, &glob_result);
	if (return_value != 0) {
		globfree(&glob_result);
		throw runtime_error("Globbing failed with return value " +
		                    to_string(return_value));
	}

	vector<filesystem::path> files;
	for (size_t i = 0; i < glob_result.gl_pathc; ++i) {
		files.push_back(filesystem::path(glob_result.gl_pathv[i]));
	}

	globfree(&glob_result);

	if (files.empty()) {
		throw runtime_error("Filesystem glob '" + glob_pattern +
		                    "' did not match any files");
	}

	return files;
}

} // namespace jonin_bt
