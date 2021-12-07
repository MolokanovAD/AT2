#pragma once
#include "Node.h"
#include "Screen.h"
#include "Brackets.h"
#include "Group.h"
#include "Repeat.h"
#include "Positive.h"
#include "Or.h"
#include "Concat.h"
#include "Leaf.h"
#include <list>
#include <stack>
#include <stdexcept>

namespace regexpr {
	typedef std::list<SP_Node>::iterator NL_It;
	typedef std::vector<std::pair<int, std::vector<int>>> FP;
	class SyntaxTree {
	private:
		std::string expr;
		std::list<SP_Node> nodeList;
		std::list<std::pair<std::shared_ptr<NL_It>, std::shared_ptr<NL_It>>> allBrackets;
		std::list<int> groups;
		std::vector<char> alphabet;
		void createNodeList();
		void buildFollowTable(SP_Node, FP& followPos);
		void printSubTree(SP_Node, int);
	public:
		SyntaxTree() {}
		SyntaxTree(const std::string& s);
		std::vector<int> firstPositions() const;
		std::string getExpression() const { return expr; }
		//void setExpr(const std::string& s) { expr = s; }
		void create(const std::string& s);
		FP buildFPTable();
		void print();
	};
	
}
