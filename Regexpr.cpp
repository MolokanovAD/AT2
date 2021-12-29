#include "Regexpr.h"
#include "SyntaxTree.h"
namespace regexpr {
	std::string intersection(const std::string& first, const std::string& second) {
		SyntaxTree firstTree(first);
		SyntaxTree secondTree(second);
		DFA firstDFA(firstTree.firstPositions(), firstTree.buildFPTable(), firstTree.getExpression(), firstTree.getAlphabet());
		DFA secondDFA(secondTree.firstPositions(), secondTree.buildFPTable(), secondTree.getExpression(), secondTree.getAlphabet());
		//return product(firstDFA.minimize(), secondDFA.minimize(), MODE::INTERSECTION).minimize().recoverExpression();//minimization
		return product(firstDFA, secondDFA, MODE::INTERSECTION).recoverExpression();
	}
	DFA intersection(const DFA& first, const DFA& second) {
		return product(first, second, MODE::INTERSECTION).minimize();//minimization
	}
	std::string regexpr::addition(const std::string& expression) {
		SyntaxTree langTree(expression);
		std::string reg("(");
		for (char s : langTree.getAlphabet()) {
			reg += s;
			reg += "|";
		}
		reg.erase(reg.length() - 1);
		reg += "){0}";
		SyntaxTree fullTree(reg);
		DFA fullDFA(fullTree.firstPositions(), fullTree.buildFPTable(), fullTree.getExpression(), fullTree.getAlphabet());
		DFA langDFA(langTree.firstPositions(), langTree.buildFPTable(), langTree.getExpression(), langTree.getAlphabet());
		return product(fullDFA.minimize(), langDFA.minimize(), MODE::DIFFERENCE).minimize().recoverExpression();//minimization
	}
	DFA addition(const DFA& dfa) {//no minimization
		std::string reg("(");
		for (char s : dfa.getAlphabet()) {
			reg += s;
			reg += "|";
		}
		reg.erase(reg.length() - 1);
		reg += "){0}";
		SyntaxTree fullTree(reg);
		DFA fullDFA(fullTree.firstPositions(), fullTree.buildFPTable(), fullTree.getExpression(), fullTree.getAlphabet());
		return product(fullDFA.minimize(), dfa, MODE::DIFFERENCE);
	}
	std::string recoverRegularExpression(const DFA& obj) {//no minimization
		return obj.recoverExpression();
	}
	DFA compile(const std::string& expression) {
		SyntaxTree tree(expression);
		DFA result(tree.firstPositions(), tree.buildFPTable(), tree.getExpression(), tree.getAlphabet());
		return result.minimize();
	}
	bool match(const std::string& expression, const std::string& s) {
		SyntaxTree tree(expression);
		DFA a(tree.firstPositions(), tree.buildFPTable(), tree.getExpression(), tree.getAlphabet());
		return a.match(s);
	}
}