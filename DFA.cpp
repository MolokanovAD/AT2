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
}
void regexpr::DFA::minimize() {
	//std::vector<std::unordered_set<SP_State>> groups;
	//groups.push_back(std::unordered_set<SP_State>());
	//groups.push_back(std::unordered_set<SP_State>());
	//for (auto i : states) {
	//	if (i->isRecieving())
	//		groups[1].insert(i);
	//	else
	//		groups[0].insert(i);
	//}
	std::vector<Split> groups;
	for (auto i : states) {
		int g = i->isRecieving();
		groups.push_back(Split(i, g, g));
	}

	for (auto i : groups) {
		for (auto symbol : alphabet) {

		}
	}
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