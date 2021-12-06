#include "Leaf.h"

void regexpr::Leaf::print() {
	std::cout << "'" ;
	if (!nullable)
		std::cout << value;
	std::cout << "'";
}
