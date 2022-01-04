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
//#include "Functions.h"
using namespace regexpr;
//fix alphabet (when named group appears)
int main() {
	SyntaxTree a,b;
	try {
		std::string test("abc{0}e{3}");
		//SyntaxTree d("a{1,44");
		std::string str1("a{0}b{0}c");
		std::string str2("abc");
		a.create(str1);
		b.create(str2);
		DFA automat1(a.firstPositions(),a.buildFPTable(),a.getExpression(),a.getAlphabet());
		DFA automat2(b.firstPositions(), b.buildFPTable(), b.getExpression(), b.getAlphabet());
		automat1.print();
		/*std::cout << automat1.recoverExpression() << std::endl;
		std::string strres(automat1.recoverExpression());
		SyntaxTree c(strres);
		DFA automatres(c.firstPositions(), c.buildFPTable(), c.getExpression(), c.getAlphabet());
		automatres.print();
		automatres.minimize();
		automatres.print();*/
		//std::cout << automat1.minimize().recoverExpression();
		//std::cout << automat1.match("");
		//automat1.print();
		//std::cout << std::endl;
		//automat1.minimize();
		/*automat1.print();
		automat2.print();*/
		//automat2.minimize();
		//automat2.print();
		//std::cout << intersection(str1, str2);
		/*DFA pr = product(automat1, automat2, MODE::INTERSECTION);
		pr.print();
		std::cout << intersection(str1, str2) << std::endl;*/
		//std::cout << intersection(automat1, automat2).recoverExpression();
		//pr.minimize().print();
	}
	catch (std::exception& a) {
		std::cout << a.what();
	}
}