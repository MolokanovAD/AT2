#include <iostream>
#include <list>
#include <stack>
#include <string>
#include <set>
#include <unordered_set>
#include <vector>
#include "SyntaxTree.h"
#include "DFA.h"
#include "Regexpr.h"
using namespace regexpr;
//fix alphabet (when named group appears)
int main() {
	SyntaxTree a,b;
	try {
		std::string str1("ab");
		std::string str2("abc{0}");
		a.create(str1);
		b.create(str2);
		DFA automat1(a.firstPositions(),a.buildFPTable(),a.getExpression(),a.getAlphabet());
		DFA automat2(b.firstPositions(), b.buildFPTable(), b.getExpression(), b.getAlphabet());
		automat1.minimize();
		automat2.minimize();
		DFA pr = product(automat1, automat2, MODE::INTERSECTION);
		//pr.print();
		std::cout << intersection(str1, str2) << std::endl;
		std::cout << intersection(automat1, automat2).recoverExpression();
		//pr.minimize().print();
	}
	catch (std::exception& a) {
		std::cout << a.what();
	}
}