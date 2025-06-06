#include "components/binding.hpp"
#include "components/rule.hpp"
#include "components/statement.hpp"
#include <iostream>
#include <ostream>
#include <vector>

using namespace jonin_bt;

int main(int argc, char **argv) {
	std::vector<std::string> v = {"This", "Is", "A", "Keylist"};
	auto result = Binding::new_Binding("Hello", v);
	if (!result)
		printf("Ah damn!\n");
	else
		std::cout << result->to_string() << std::endl;

	auto rule = Rule::new_Rule("Somerule", "clang --version");
	if (!result)
		printf("Ah damn!\n");
	else
		std::cout << rule->to_string() << std::endl;

	std::vector<std::string> vin = {"somecfile.c"};
	std::vector<std::string> vout = {"somecobject"};
	auto statement = Statement::new_Statement(rule.value(), vin, vout);
	if (!result)
		printf("Ah damn!\n");
	else
		std::cout << statement->to_string() << std::endl;

	return 0;
}
