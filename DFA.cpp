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
	std::vector<Split> oldStates;
	int groupsTotal = 2;
	for (auto i : states) {
		int g = i->isRecieving();
		if (!i->isError())
			oldStates.push_back(Split(i, g, g, {}));
		else {
			oldStates.push_back(Split(i, groupsTotal, groupsTotal, {}));
			groupsTotal++;
		}
	}
	bool changed = false;
	do {
		changed = false;
		for (auto symbol : alphabet) {
			for (auto& state : oldStates) {
				auto to = std::get<0>(state)->transmit(symbol);
				for (auto i : oldStates) {
					//searching for destination in set
					if (std::get<0>(i).get() == to.get()) {
						std::get<3>(state)[symbol] = std::get<2>(i);
						break;
					}
				}
			}
			int oldGroupsTotal = groupsTotal;
			for (int g = 0; g < oldGroupsTotal; g++) {
				bool needNewGroup = false;
				int first = -1;
				for (auto& i : oldStates) {
					if (std::get<2>(i) == g) {
						if (first == -1)
							first = std::get<3>(i)[symbol];
						else if (std::get<3>(i)[symbol] != first) {
							std::get<2>(i) = groupsTotal;
							needNewGroup = true;
						}
					}
				}
				if (needNewGroup) {
					changed = true;
					groupsTotal++;
					break;
				}
			}
		}
	} while (changed);
	states.clear();
	std::vector<std::map<char,int>> transit;
	for (int g = 0; g < groupsTotal; g++) {
		std::unordered_set<int> pos;
		bool rec = false;
		Split s;
		for (auto& i : oldStates) {
			if (std::get<2>(i) == g) {
				s = i;
				for (auto p : std::get<0>(i)->getPositions())
					pos.insert(p);
				rec = std::get<0>(i)->isRecieving();
			}
		}
		states.push_back(std::make_shared<State>(pos, rec));
		transit.push_back(std::get<3>(s));
	}
	for (char c : alphabet) {
		auto j = transit.begin();
		for (auto i = states.begin(); i < states.end();i++,j++) {
			(*i)->addTransition(c, states[(*j)[c]]);
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