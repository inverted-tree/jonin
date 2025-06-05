#include "components/binding.hpp"
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
	return 0;
}
