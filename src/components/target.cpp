#include "target.hpp"
#include <expected>
#include <filesystem>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <system_error>

namespace jonin_bt {
using namespace std;

auto split_optional_string(optional<string> &str) -> optional<vector<string>>;
auto expand_path(string &glob) -> vector<filesystem::path>;

Target::Target(
    string name_, Language language_, optional<string> compiler_,
    optional<vector<string>> cflags_, optional<vector<string>> cxxflags_,
    optional<vector<string>> fcflags_, optional<vector<string>> ldflags_,
    optional<vector<string>> cppflags_, optional<vector<string>> asflags_,
    optional<vector<string>> arflags_, vector<filesystem::path> source_files_,
    optional<vector<string>> dependencies_)
    : name(name_), language(language_), compiler(compiler_), cflags(cflags_),
      cxxflags(cxxflags_), fcflags(fcflags_), ldflags(ldflags_),
      cppflags(cppflags_), asflags(asflags_), arflags(arflags_),
      source_files(source_files_), dependencies(dependencies_) {
	if (name.empty())
		throw invalid_argument("A targets name must not be empty");
	for (filesystem::path file : source_files)
		if (!filesystem::exists(file))
			throw invalid_argument("Source file '" + string(file) +
			                       "' does not exist");
}

auto strview_tolower(string_view sv) -> string {
	string result(sv);
	transform(result.begin(), result.end(), result.begin(),
	          [](unsigned char c) { return tolower(c); });
	return result;
}

auto Target::new_Target(
    std::map<std::string, std::optional<std::string>> target_options)
    -> std::expected<Target, std::string> {
	string name = target_options["name"].value_or("Default");

	Language language = [](string_view l) -> Language {
		string lang = strview_tolower(l);
		if (lang == "undefined")
			return Language::UNDEFINED;
		else if (lang == "c")
			return Language::C;
		else if (lang == "c++" || lang == "cpp" || lang == "cxx")
			return Language::CPP;
		else if (lang == "fortran" || lang == "f90" || lang == "f95")
			return Language::FORTRAN;
		else
			throw invalid_argument("Unknown language '" + lang + "'");
	}(target_options["language"].value_or("undefined"));

	optional<string> compiler = target_options["compiler"];

	optional<vector<string>> cflags =
	    split_optional_string(target_options["cflags"]);

	optional<vector<string>> cxxflags =
	    split_optional_string(target_options["cxxflags"]);

	optional<vector<string>> fcflags =
	    split_optional_string(target_options["fcflags"]);

	optional<vector<string>> ldflags =
	    split_optional_string(target_options["ldflags"]);

	optional<vector<string>> cppflags =
	    split_optional_string(target_options["cppflags"]);

	optional<vector<string>> asflags =
	    split_optional_string(target_options["asflags"]);

	optional<vector<string>> arflags =
	    split_optional_string(target_options["arflags"]);

	optional<vector<string>> _paths =
	    split_optional_string(target_options["files"]);
	vector<filesystem::path> source_files;
	if (_paths.has_value()) {
		for (string glob : _paths.value()) {
			vector<filesystem::path> _expanded = expand_path(glob);
			source_files.insert(source_files.end(), _expanded.begin(),
			                    _expanded.end());
		}
	} else {
		// TODO: If the language field is set, throw in anything from /src with
		// that language suffix. If not, try to infer the language.
		;
	}

	optional<vector<string>> dependencies =
	    split_optional_string(target_options["dependencies"]);

	try {
		return Target(name, language, compiler, cflags, cxxflags, fcflags,
		              ldflags, cppflags, asflags, arflags, source_files,
		              dependencies);
	} catch (invalid_argument const &e) {
		return unexpected(string(e.what()));
	}
}

// TODO: If this implementation gets more complicated make this its own module.

auto expand_path(string &glob) -> vector<filesystem::path> {
	vector<string> components;
	string current;

	for (char c : glob)
		if (c == '/') {
			if (!current.empty()) {
				components.push_back(current);
				current.clear();
			}
		} else {
			current += c;
		}

	if (!current.empty())
		components.push_back(current);

	// TODO: Implement the file system globbing. To get something working, we
	// just translate the given glob directly to a filename and check if it
	// exists. If that is the case just return this filepath, else throw an
	// error.
	vector<filesystem::path> files;
	if (filesystem::exists(glob))
		files.push_back(glob);
	else
		throw runtime_error("Filesystem glob '" + glob +
		                    "' did not match any files");

	return files;
}

auto split_optional_string(optional<string> &str) -> optional<vector<string>> {
	if (!str)
		return nullopt;

	istringstream iss(*str);
	vector<string> result;
	string word;

	while (iss >> word)
		result.push_back(word);

	return result.empty() ? nullopt : optional(result);
}

} // namespace jonin_bt
