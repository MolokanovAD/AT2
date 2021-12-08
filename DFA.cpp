#include "DFA.h"
#include "Functions.h"
regexpr::DFA::DFA(const std::unordered_set<int>& first, std::vector<std::pair<int, std::unordered_set<int>>> followPos, const std::string& s, std::unordered_set<char> a):expr(s),alphabet(a) {
	states.push_back(std::make_shared<State>(first, std::find(first.begin(), first.end(), expr.length()) != first.end()));
	SP_State S = *(states.begin());
	while (S.get()) {
		for (auto c : alphabet) {
			//for each symbol in the alphabet
			std::unordered_set<int> newStatePositions;
			for (auto pos : S->getPositions()) {
				if (pos == expr.length())
					continue;
				if (*(expr.begin() + pos) == c) {
					//found position which refers to requires symbol from the alphabet
					for (auto fpLine : followPos) {
						if (fpLine.first == pos) {
							newStatePositions.insert(fpLine.second.begin(), fpLine.second.end());
							break;
						}
					}
				}
			}
			auto state = states.begin();
			for (; state < states.end();state++) {
				if ((*state)->getPositions() == newStatePositions) {
					S->addTransition(c, *(state));
					break;
				}
			}
			if (state == states.end()) {
				//if there is no states with such set of positions
				states.push_back(std::make_shared<State>(newStatePositions, newStatePositions.find(expr.length()) != newStatePositions.end()));
				S->addTransition(c, *(--states.end()));
			}
		}
		S->process();
		S = firstNotProcessed();
	}
	//while (!S->isNotProcessed()) {
	//	std::vector<int> newStatePositions;
	//	bool foundFPLine = false;
	//	for (auto i : S->getPositions()) {
	//		//for each position
	//		for (auto j : followPos) {
	//			//for each line in FP 
	//			if (i == j.first) {
	//				foundFPLine = true;
	//				for (int k : j.second) {
	//					//for each follow position
	//					if (std::find(newStatePositions.begin(), newStatePositions.end(), k) == newStatePositions.end()) {
	//						//if there is no such position in newStatePositions
	//						newStatePositions.push_back(k);
	//					}
	//				}
	//				break;
	//			}
	//		}
	//	}
	//	if (foundFPLine) {
	//		//if there are transitions from the state
	//		auto i = states.begin();
	//		for (; i < states.end(); i++) {
	//			if ((*i)->samePositions(newStatePositions)) {
	//				break;
	//			}
	//		}
	//		auto pos = S->getPositions();
	//		auto last = std::find(pos.begin(), pos.end(), expr.length());
	//		if (last != pos.end())
	//			pos.erase(last);
	//		if (i == states.end()) {
	//			states.push_back(std::make_shared<State>(newStatePositions, std::find(newStatePositions.begin(), newStatePositions.end(), expr.length()) != newStatePositions.end()));
	//			S->addTransition(pos, *(--states.end()));
	//		}
	//		else
	//			S->addTransition(pos, *i);
	//	}
	//	S->process();
	//	S = *(--states.end());
	//}
}
std::shared_ptr<regexpr::State> regexpr::DFA::firstNotProcessed() {
	auto i = states.begin();
	while (i < states.end() && (*i)->isProcessed()) {
		i++;
	}
	return i == states.end() ? SP_State(nullptr) : *i;
}
void regexpr::DFA::print() {
	int k = 0;
	for (auto i : states) {
		if (i->isRecieving())
			std::cout << "R  ";
		std::cout << "{ ";
		for (auto j : i->getPositions()) {
			std::cout << j << ":";
			if (j < expr.length())
				std::cout << *(expr.begin() + j) << " ";
		}
		std::cout << "}" << std::endl <<  "transitions:  ";
		for (auto j : i->getTransitions()) {
			std::cout << std::endl << "        ( " << j.first << " " << ")    ->      { ";
			for(auto m: j.second->getPositions()) {
				std::cout << m << ":";
				if (m < expr.length())
					std::cout << *(expr.begin() + m) << " ";
				else std::cout << '#';
			}
			std::cout << "};";
		}
		std::cout << std::endl;
	}
}