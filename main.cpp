#include <iostream>
#include <list>
#include <stack>
#include <string>
#include <vector>
#include "SyntaxTree.h"
using namespace regexpr;
int main() {
	SyntaxTree a;
	std::vector<int> i(3);
	try {
		a.create("(m|e)p(h|i)");
		a.print();
		a.buildDFA();
	}
	catch (std::exception& a) {
		std::cout << a.what();
	}
}