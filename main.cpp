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
#include "Functions.h"
using namespace regexpr;
//fix alphabet (when named group appears)
int main() {
	SyntaxTree a,b;
	try {
		std::string str1("a{0}b{0}c");
		std::string str2("abc+");
		a.create(str1);
		b.create(str2);
		//DFA automat1 = compile(str1);
		DFA automat1(a.firstPositions(),a.buildFPTable(),a.getExpression(),a.getAlphabet());
		automat1.minimize(); 
		//DFA automat2 = compile(str2);
		DFA automat2(b.firstPositions(), b.buildFPTable(), b.getExpression(), b.getAlphabet());
		automat2.minimize();
		//automat1.print();
		b.create(recoverRegularExpression(automat1));
		DFA automat3(a.firstPositions(), a.buildFPTable(), a.getExpression(), a.getAlphabet());
		automat3.minimize();
		//automat3.print();
		DFA pr = product(automat1, automat2, MODE::INTERSECTION);
		pr.minimize().print();
		std::cout << intersection(str1, str2) << std::endl;
		//std::cout << intersection(automat1, automat2).recoverExpression();
		//pr.minimize().print();
	}
	catch (std::exception& a) {
		std::cout << a.what();
	}
}