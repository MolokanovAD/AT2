#include "SyntaxTree.h"
#include "LastNode.h"

namespace regexpr {
	regexpr::SyntaxTree::SyntaxTree(const std::string& s) {
		create(s);
	}
	FP SyntaxTree::buildFPTable() {
		if (nodeList.size() != 1)
			throw std::exception("Syntax tree is not created");
		FP followPos;
		buildFollowTable(nodeList.front(),followPos);
		return followPos;
	}
	void regexpr::SyntaxTree::buildFollowTable(SP_Node n, FP& followPos) {
		auto binary = std::dynamic_pointer_cast<BinaryOperator>(n);
		if (binary) {
			buildFollowTable(binary->getLeftChild(),followPos);
			buildFollowTable(binary->getRightChild(),followPos);
			binary->buildFollowPos(followPos);
		}
		auto unary = std::dynamic_pointer_cast<UnaryOperator>(n);
		if (unary) {
			buildFollowTable(unary->getChild(), followPos);
			unary->buildFollowPos(followPos);
		}
	}
	void SyntaxTree::createNodeList() {
		allBrackets.clear();
		nodeList.clear();
		groups.clear();
		for (auto i = expr.begin(); i < expr.end(); i++) {
			switch (*i) {
				case '#': {
						if ((i + 1) != expr.end()) {
							//if there are some symbols after '#'
							i++;
							nodeList.emplace_back(std::make_shared<Leaf>(i-expr.begin(),*i));
							if (std::find(alphabet.begin(), alphabet.end(), *i) == alphabet.end())
								//if there's no such symbol in alphabet, it's being put there
								alphabet.push_back(*i);
							break;
						}
						else throw std::exception("Syntax error");
					}
				case '(': {
						nodeList.emplace_back(std::make_shared<OpenBracket>());
						allBrackets.emplace_front(std::pair<std::shared_ptr<NL_It>, std::shared_ptr<NL_It>>(std::make_shared<NL_It>(--nodeList.end()), nullptr));
						break;
					}
				case ')': {
						nodeList.emplace_back(std::make_shared<CloseBracket>());
						auto iter = allBrackets.begin();
						while (iter != allBrackets.end() && (*iter).second)iter++;
						if (iter == allBrackets.end())throw std::exception("Syntax error");
						(*iter).second = std::make_shared<NL_It>(--nodeList.end());
						break;
					}
				case '{': {
						std::string fr("");
						int from, to = INT_MAX;
						i++;
						while (std::isdigit(*i)) {
							//forming string of digits which is the lower limit
							fr += *i;
							i++;
						}
						try {
							//parsing the string
							from = std::stoi(fr);
						}
						catch (std::exception& e) {
							throw e;
						}
						if (*i == ',') {
							//if ',' is met there is a second limit
							//repeat same thing
							i++;
							fr.clear();
							while (std::isdigit(*i)) {
								fr += *i;
								i++;
							}
							try {
								to = std::stoi(fr);
							}
							catch (std::exception& e) {
								throw e;
							}
						}
						if (*i == '}') {
							nodeList.emplace_back(std::make_shared<Repeat>(from, to));
							break;
						}
						else throw std::exception("Syntax error");
					}
				case '+': {
						nodeList.emplace_back(std::make_shared<Positive>());
						break;
					}
				case '.': {
						nodeList.emplace_back(std::make_shared<Concat>());
						break;
					}
				case '|': {
						nodeList.emplace_back(std::make_shared<Or>());
						break;
					}
				case '^': {
						nodeList.emplace_back(std::make_shared<Leaf>());
						break;
					}
				default: {
						nodeList.emplace_back(std::make_shared<Leaf>(i-expr.begin(),*i));
						if (std::find(alphabet.begin(), alphabet.end(), *i) == alphabet.end())
							alphabet.push_back(*i);
					}
			}
		}
		if (allBrackets.back().second == nullptr)throw std::exception("Syntax error");
		nodeList.emplace_front(std::make_shared<OpenBracket>());
		nodeList.emplace_back(std::make_shared<LastNode>(expr.length()));
		//pushing lastnode for RE->DFA algorithm
		nodeList.emplace_back(std::make_shared<CloseBracket>());
		allBrackets.emplace_back(std::pair<std::shared_ptr<NL_It>, std::shared_ptr<NL_It>>(std::make_shared<NL_It>(nodeList.begin()), std::make_shared<NL_It>(--nodeList.end())));
	}


	void SyntaxTree::create(const std::string& s) {
		expr = s;
		NL_It openBracket, closeBracket, scanFrom;
		createNodeList();
		while(!allBrackets.empty()){
			openBracket = *(*allBrackets.begin()).first;
			closeBracket = *(*allBrackets.begin()).second;
			scanFrom = openBracket;
			scanFrom++;
			std::string number("");
			bool isGroup = false;
			int num;//group number
			while (true) {//numbered group check
				SP_Leaf n = std::dynamic_pointer_cast<Leaf>(*scanFrom);
				char symbol;
				if (n) {
					symbol = (*n).getValue();
					if (std::isdigit(symbol)) {
						number += symbol;
					}
					else if (symbol == ':' && !number.empty()) {
						scanFrom++;
						if (scanFrom != closeBracket) {
							//this is a numbered group
							isGroup = true;
							num = std::stoi(number);
							auto f = openBracket;
							f++;
							//delete n: leafs from nodeList
							nodeList.erase(f, scanFrom);
							break;
						}
						else throw std::exception("Syntax error");//(1:) situation
					}
					else break;//average brackets
				}
				else break;//average brackets
				scanFrom++;
			}
			if(scanFrom != closeBracket) scanFrom++;
			for (auto i = scanFrom; i != closeBracket; i++) {//positive closure and repeat check
				SP_Pos positiveNode = std::dynamic_pointer_cast<Positive>(*i);
				auto pred = i;
				pred--;
				if (positiveNode && positiveNode->isNotProcessed()) {
					if ((*pred)->isNotProcessed()) throw std::exception("Syntax error");
					positiveNode->setChild(*pred);
					(*pred)->setParent(positiveNode);
					positiveNode->buildPositions();
					nodeList.erase(pred);
				}
				else {
					SP_Rep repeatNode = std::dynamic_pointer_cast<Repeat>(*i);
					if (repeatNode && repeatNode->isNotProcessed()) {
						if ((*pred)->isNotProcessed()) throw std::exception("Syntax error");
						repeatNode->setChild(*pred);
						(*pred)->setParent(repeatNode);
						repeatNode->buildPositions();
						nodeList.erase(pred);
					}
				}

			}
			//going back to scan the string again
			scanFrom = openBracket;
			scanFrom++;
			scanFrom++;
			for (auto i = scanFrom; i != closeBracket; i++) {//concatenation check
				auto pred = i;
				pred--;
				auto concatNode = std::dynamic_pointer_cast<Concat>(*i);
				if (concatNode && (*concatNode).isNotProcessed()) {//if . metasymbol is met
					auto next = i;
					next++;
					if ((*next)->isNotProcessed() || (*pred)->isNotProcessed()) throw std::exception("Syntax error");
					(*concatNode).setChildren(*pred, *next);
					(*pred)->setParent(concatNode);
					(*next)->setParent(concatNode);
					concatNode->buildPositions();
					nodeList.erase(pred);
					nodeList.erase(next);
				}
				else {
					//if non-processed OR-node is met we don't touch it
					if (std::dynamic_pointer_cast<Or>(*i) && (*i)->isNotProcessed() || std::dynamic_pointer_cast<Or>(*pred) && (*pred)->isNotProcessed()) continue;
					//otherwise, if we have empty not OR node, it's a mistake
					if((*pred)->isNotProcessed() || (*i)->isNotProcessed()) throw std::exception("Syntax error");
					auto concNode = std::make_shared<Concat>(*pred, *i);
					concNode->buildPositions();
					nodeList.emplace(pred, concNode);
					(*pred)->setParent(concNode);
					(*i)->setParent(concNode);
					nodeList.erase(pred);
					pred = i;
					i++;
					nodeList.erase(pred);
					i--;
				}
			}
			scanFrom = openBracket;
			scanFrom++;
			scanFrom++;
			for (auto i = scanFrom; i != closeBracket; i++) {//or check
				auto orNode = std::dynamic_pointer_cast<Or>(*i);
				if (orNode && orNode->isNotProcessed()) {
					auto pred = i;
					pred--;
					auto next = i;
					next++;
					if ((*pred)->isNotProcessed() || (*next)->isNotProcessed()) throw std::exception("Syntax error");
					orNode->setChildren(*pred, *next);
					orNode->buildPositions();
					(*pred)->setParent(orNode);
					(*next)->setParent(orNode);
					nodeList.erase(pred);
					nodeList.erase(next);
				}
			}
			allBrackets.erase(allBrackets.begin());
			//popping out pair of brackets which was just processed
			if (isGroup) {
				scanFrom--;
				auto gr = std::make_shared<Group>(*scanFrom, num);
				gr->buildPositions();
				nodeList.emplace(scanFrom, gr);
				if (std::find(groups.begin(), groups.end(), num) != groups.end()) throw std::exception("Group with this number already exists");
				groups.emplace_back(num);
				nodeList.erase(scanFrom);
			}
			nodeList.erase(openBracket);
			nodeList.erase(closeBracket);
		}
	}



	void regexpr::SyntaxTree::print() {
		if (nodeList.size() == 1) 
			printSubTree(nodeList.front(), 0);
		else 
			throw std::exception("Syntax tree is not created");
	}
	void regexpr::SyntaxTree::printSubTree(SP_Node ptr, int a) {
			if (ptr) {
				a = a + 1;
				auto con = std::dynamic_pointer_cast<Concat>(ptr);
				if (con) {
					printSubTree(con->getRightChild(), a);
					for (int i = 0; i < 2 * (a - 1); i++)
						std::cout << " ";
					ptr->print();
					std::cout << std::endl;
					printSubTree(con->getLeftChild(), a);
				}
				else {
					auto orNode = std::dynamic_pointer_cast<Or>(ptr);
					if (orNode) {
						printSubTree(orNode->getRightChild(), a);
						for (int i = 0; i < 2 * (a - 1); i++)
							std::cout << " ";
						ptr->print();
						std::cout << std::endl;
						printSubTree(orNode->getLeftChild(), a);
					}
					else {
						auto repNode = std::dynamic_pointer_cast<Repeat>(ptr);
						if (repNode) {
							printSubTree(repNode->getChild(), a);
							for (int i = 0; i < 2 * (a - 1); i++)
								std::cout << " ";
							ptr->print();
							std::cout << std::endl;
						}
						else {
							auto posNode = std::dynamic_pointer_cast<Positive>(ptr);
							if (posNode) {
								printSubTree(posNode->getChild(), a);
								for (int i = 0; i < 2 * (a - 1); i++)
									std::cout << " ";
								ptr->print();
								std::cout << std::endl;
							}
							else {
								auto leaf = std::dynamic_pointer_cast<Leaf>(ptr);
								if (leaf) {
									for (int i = 0; i < 2 * (a - 1); i++)
										std::cout << " ";
									leaf->print();
									std::cout << std::endl;
								}
								else {

									auto group = std::dynamic_pointer_cast<Group>(ptr);
									if (group) {
										printSubTree(group->getChild(), a);
										for (int i = 0; i < 2 * (a - 1); i++)
											std::cout << " ";
										group->print();
										std::cout << std::endl;
									}
								}
							}
						}
					}
				}
			}
	}
	std::vector<int> SyntaxTree::firstPositions() const {
		if(nodeList.size() == 1)
			return nodeList.front()->firstPositions();
		throw std::exception("Syntax tree is not created.");
	}
}