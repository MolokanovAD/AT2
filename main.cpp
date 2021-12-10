#include <iostream>
#include <list>
#include <stack>
#include <string>
#include <set>
#include <unordered_set>
#include <vector>
#include "SyntaxTree.h"
#include "DFA.h"
using namespace regexpr;
//fix alphabet (when named group appears)
int main() {
	SyntaxTree a;
	try {
		a.create("(m|e)p(h|i)");
		a.print();
		DFA automat(a.firstPositions(),a.buildFPTable(),a.getExpression(),a.getAlphabet());
		automat.minimize();
		automat.print();
	}
	catch (std::exception& a) {
		std::cout << a.what();
	}
}