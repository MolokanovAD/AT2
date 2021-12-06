#include <iostream>
#include <list>
#include <stack>
#include <string>
#include <vector>
#include "SyntaxTree.h"
#include "DFA.h"
using namespace regexpr;
int main() {
	SyntaxTree a;
	try {
		a.create("(m|e)p(h|i)");
		a.print();
		DFA automat(a.firstPositions(),a.buildFPTable(),a.getExpression());
	}
	catch (std::exception& a) {
		std::cout << a.what();
	}
}