#include "DFA.h"
#include "Functions.h"
regexpr::DFA::DFA(const std::vector<int>& first, std::vector<std::pair<int, std::vector<int>>> followPos, const std::string& s):expr(s) {
	states.push_back(std::make_shared<State>(first, std::find(first.begin(), first.end(), expr.length()) != first.end()));
	auto S = *(--states.end());
	while (!S->isNotProcessed()) {
		std::vector<int> newStatePositions;
		bool foundFPLine = false;
		for (auto i : S->getPositions()) {
			//for each position
			for (auto j : followPos) {
				//for each line in FP 
				if (i == j.first) {
					foundFPLine = true;
					for (int k : j.second) {
						//for each follow position
						if (std::find(newStatePositions.begin(), newStatePositions.end(), k) == newStatePositions.end()) {
							//if there is no such position in newStatePositions
							newStatePositions.push_back(k);
						}
					}
					break;
				}
			}
		}
		if (foundFPLine) {
			//if there are transitions from the state
			auto i = states.begin();
			for (; i < states.end(); i++) {
				if ((*i)->samePositions(newStatePositions)) {
					break;
				}
			}
			if (i == states.end()) {
				states.push_back(std::make_shared<State>(newStatePositions, std::find(newStatePositions.begin(), newStatePositions.end(), expr.length()) != newStatePositions.end()));
				S->addTransition(S->getPositions(), *(--states.end()));
			}
			else
				S->addTransition(S->getPositions(), *i);
		}
		S->process();
		S = *(--states.end());
	}
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
		std::cout << "}  transitions:  ";
		for (auto j : i->getTransitions()) {
			std::cout << "THIS   ->    { ";
			for (auto m : j.first) {
				std::cout << m << ":";
				if (m < expr.length())
					std::cout << *(expr.begin() + m) << " ";
			}
			std::cout << "}    ->      { ";
			for(auto m: j.second->getPositions()) {
				std::cout << m << ":";
				if (m < expr.length())
					std::cout << *(expr.begin() + m) << " ";
			}
			std::cout << "};";
		}
		std::cout << std::endl;
	}
}