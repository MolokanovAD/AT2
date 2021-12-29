#include "DFA.h"
#include "Functions.h"
namespace regexpr {
	DFA::DFA(const std::unordered_set<int>& first, std::vector<std::pair<int, std::unordered_set<int>>> followPos, const std::string& s, std::unordered_set<char> a) :expr(s), alphabet(a) {
		//states.push_back(std::make_shared<State>(first, std::find(first.begin(), first.end(), expr.length()) != first.end(),1));
		states.push_back(std::make_shared<State>(first, std::find(first.begin(), first.end(), expr.length()) != first.end(), 0));
		startState = states.front();
		//int n = 2;
		int n = 1;
		SP_State S = *(states.begin());
		while (S.get()) {
			for (auto c : alphabet) {
				//for each symbol in the alphabet
				std::unordered_set<int> newStatePositions;
				for (auto pos : S->getPositions()) {
					if (pos == expr.length())
						continue;
					if (*(expr.begin() + pos) == c) {
						//found position which refers to required symbol from the alphabet
						for (auto fpLine : followPos) {
							if (fpLine.first == pos) {
								newStatePositions.insert(fpLine.second.begin(), fpLine.second.end());
								break;
							}
						}
					}
				}
				auto state = states.begin();
				for (; state < states.end(); state++) {
					if ((*state)->getPositions() == newStatePositions) {
						S->addTransition(c, *(state));
						break;
					}
				}
				if (state == states.end()) {
					//if there is no states with such set of positions
					states.push_back(std::make_shared<State>(newStatePositions, newStatePositions.find(expr.length()) != newStatePositions.end(), n));
					n++;
					S->addTransition(c, *(--states.end()));
				}
			}
			S->process();
			S = firstNotProcessed();
		}
		//std::sort(states.begin(), states.end(), [](SP_State a, SP_State b) {return a->getNumber() < b->getNumber(); });
	}

	DFA DFA::minimize() {
		//std::vector<Split> oldStates;
		//int groupsTotal = 2;
		//for (auto i : states) {
		//	int g = i->isRecieving();
		//	if (!i->isError())
		//		oldStates.push_back(Split(i, g, g, {}));
		//	else {
		//		oldStates.push_back(Split(i, groupsTotal, groupsTotal, {}));
		//		groupsTotal++;
		//	}
		//}
		//bool changed = false;
		//do {
		//	changed = false;
		//	for (auto symbol : alphabet) {
		//		for (auto& state : oldStates) {
		//			auto to = std::get<0>(state)->transmit(symbol);
		//			for (auto i : oldStates) {
		//				//searching for destination in set
		//				if (std::get<0>(i).get() == to.get()) {
		//					std::get<3>(state)[symbol] = std::get<2>(i);
		//					break;
		//				}
		//			}
		//		}
		//		int oldGroupsTotal = groupsTotal;
		//		for (int g = 0; g < oldGroupsTotal; g++) {
		//			bool needNewGroup = false;
		//			int first = -1;
		//			for (auto& i : oldStates) {
		//				if (std::get<2>(i) == g) {
		//					if (first == -1)
		//						first = std::get<3>(i)[symbol];
		//					else if (std::get<3>(i)[symbol] != first) {
		//						std::get<2>(i) = groupsTotal;
		//						needNewGroup = true;
		//					}
		//				}
		//			}
		//			if (needNewGroup) {
		//				changed = true;
		//				groupsTotal++;
		//				break;
		//			}
		//		}
		//	}
		//} while (changed);
		//states.clear();
		//std::vector<std::map<char, int>> transit;
		//for (int g = 0; g < groupsTotal; g++) {
		//	std::unordered_set<int> pos;
		//	bool rec = false;
		//	bool isStart = false;
		//	Split s;
		//	for (auto& i : oldStates) {
		//		if (std::get<2>(i) == g) {
		//			isStart = std::get<0>(i).get() == startState.get();
		//			s = i;
		//			for (auto p : std::get<0>(i)->getPositions())
		//				pos.insert(p);
		//			rec = std::get<0>(i)->isRecieving();
		//		}
		//	}
		//	states.push_back(std::make_shared<State>(pos, rec, g));
		//	if (isStart)
		//		startState = states.back();
		//	//states.push_back(std::make_shared<State>(pos, rec, g + 1));
		//	transit.push_back(std::get<3>(s));
		//}
		//for (char c : alphabet) {
		//	auto j = transit.begin();
		//	for (auto i = states.begin(); i < states.end(); i++, j++) {
		//		(*i)->addTransition(c, states[(*j)[c]]);
		//	}
		//}
		//std::sort(states.begin(), states.end(), [](SP_State a, SP_State b) {return a->getNumber() < b->getNumber(); });
		//return *this;
		std::vector<Split> oldStates;
		int groupsTotal = 1;
		if (states.size() == 1)
			return *this;
		bool isStartRec = startState->isRecieving();
		for (auto i : states) {
			if (i->isRecieving() == isStartRec) {
				oldStates.push_back(Split(i, 0, 0, {}));
			}	
			else {
				oldStates.push_back(Split(i, 0, groupsTotal, {}));
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
				bool addedGroup = false;
				for (int group = 0; group < groupsTotal && !addedGroup; group++) {
					auto i = oldStates.begin();
					for (; i < oldStates.end() && std::get<2>(*i) != group; i++) {}
					int destination = std::get<3>(*i)[symbol];
					for (; i < oldStates.end(); i++) {
						if (std::get<2>(*i) == group && std::get<3>(*i)[symbol] != destination) {
							std::get<1>(*i) = std::get<2>(*i);
							std::get<2>(*i) = groupsTotal;
							addedGroup = true;
							changed = true;
						}
					}
				}
				if (addedGroup)
					groupsTotal++;
			}
		} while (changed);
		states.clear();
		for(int g = 0; g < groupsTotal; g++)
			states.push_back(std::make_shared<State>());
		for (int g = 0; g < groupsTotal; g++) {
			for (auto& i : oldStates) {
				if (std::get<2>(i) == g) {
					states[g]->setRecieving(std::get<0>(i)->isRecieving());
					states[g]->setNumber(g);
					for (auto symbol : alphabet) {
						states[g]->addTransition(symbol, states[std::get<3>(i)[symbol]]);
					}
					break;
				}
			}
		}
		startState = states.front();
		return *this;
	}

	bool regexpr::DFA::match(const std::string& str) {
		auto state = startState;
		for (char s : str) {
			if (alphabet.find(s) == alphabet.end())
				throw std::exception("Syntax error");
			state = state->transmit(s);
		}
		return state->isRecieving();
	}

	std::shared_ptr<State> DFA::firstNotProcessed() {
		auto i = states.begin();
		while (i < states.end() && (*i)->isProcessed()) {
			i++;
		}
		return i == states.end() ? SP_State(nullptr) : *i;
	}
	std::string DFA::recoverExpression() const{
		vector3d matrix(states.size(), std::vector<std::vector<std::shared_ptr<std::string>>>(states.size(), std::vector<std::shared_ptr<std::string>>(states.size(), nullptr)));
		std::set<char> metaSymbols = { ')','(', '#', '+','^','.','|','{','}' };
		std::string result = "", newPath = "";
		buildBasis(matrix, metaSymbols);
		for (auto state : states) {
			if (state->isRecieving()) {
				newPath = kPath(matrix, startState, state, states.size() - 1);
				if (!result.empty() && !newPath.empty())
					result += "|";
				result += newPath;
				newPath.clear();
				
			}
		}
		return result;
	}

	void DFA::buildBasis(vector3d& matrix, std::set<char> metaSymbols) const{
		for (auto state : states) {
			int i = state->getNumber();
			for (auto& transition : state->getTransitions()) {
				int j = transition.second->getNumber();
				std::string sign("");
				sign += transition.first;
				if (metaSymbols.find(transition.first) != metaSymbols.end())
					sign.insert(sign.begin(), '#');
				if (!matrix[i][j][0].get())
					matrix[i][j][0] = std::make_shared<std::string>(sign);
				else {
					if ((*matrix[i][j][0])[0] != '(') //if there is no open breacket, then we add it
						matrix[i][j][0]->insert(matrix[i][j][0]->begin(), '(');
					else //if there is a bracket, then there is a bracket at the end, so we delete it to add a new transition as OR 
						matrix[i][j][0]->erase(matrix[i][j][0]->length() - 1);
					matrix[i][j][0]->append("|" + sign + ")");
				}
			}
			if (!matrix[i][i][0].get())
				matrix[i][i][0] = std::make_shared<std::string>("^");
			else {
				matrix[i][i][0]->insert(matrix[i][i][0]->begin(), '(');
				//*matrix[i][i][0] += "|^)";
				*matrix[i][i][0] += "|^){0}";
			}
		}
	}

	std::string DFA::kPath(vector3d& matrix, SP_State iState, SP_State jState, int k) const{
		int i = iState->getNumber();
		int j = jState->getNumber();
		if (k == 0)
			return matrix[i][j][k].get() ? *matrix[i][j][k] : "";
		if (matrix[i][j][k].get())
			return *matrix[i][j][k];
		std::string result = matrix[i][j][k - 1].get() ? *matrix[i][j][k - 1] : kPath(matrix, iState, jState, k - 1);
		std::string firstPart = matrix[i][k][k - 1].get() ? *matrix[i][k][k - 1] : kPath(matrix, iState, states[k], k - 1);
		std::string lastPart = matrix[k][j][k - 1].get() ? *matrix[k][j][k - 1] : kPath(matrix, states[k], jState, k - 1);
		if (!firstPart.empty() && !lastPart.empty()) {
			std::string repeat = matrix[k][k][k - 1].get() ? *matrix[k][k][k - 1] : kPath(matrix, states[k], states[k], k - 1);
			if (!repeat.empty() && repeat != "^") {
				if (repeat.front() == '(' && repeat.back() == ')')
					firstPart += repeat + "{0}";
				else
					firstPart += "(" + repeat + "){0}";
			}
			if (!result.empty()) {
				if (result.front() == '(' && repeat.back() == ')')
					result.erase(result.length() - 1);
				else
					result.insert(result.begin(), '(');
				result += "|" + firstPart + lastPart + ")";
			}
			else
				result += firstPart + lastPart;
		}
		matrix[i][j][k] = std::make_shared<std::string>(result);
		return result;
	}

	DFA product(const DFA& first, const DFA& second, MODE mode) {
		auto fSize = first.states.size(), sSize = second.states.size();
		auto fBegin = first.states.begin(), sBegin = second.states.begin();
		std::vector<SP_State> states(fSize * sSize);
		for (int i = 0; i < states.size(); i++)
			states[i] = std::make_shared<State>(i);
		std::unordered_set<char> alphabet = first.alphabet;
		alphabet.insert(std::make_move_iterator(second.alphabet.begin()), std::make_move_iterator(second.alphabet.end()));
		for (auto& i : first.states) {
			for (auto& j : second.states) {
				int index = i->getNumber() * sSize + j->getNumber();/*
				states[index]->setNumber(index);*/
				if (mode == DIFFERENCE && i->isRecieving() && !j->isRecieving())
					states[index]->setRecieving(true);
				else if(mode == INTERSECTION && i->isRecieving() && j->isRecieving())
					states[index]->setRecieving(true);
				for (auto& iTrans : i->getTransitions()) {
					for (auto& jTrans : j->getTransitions())
						if(iTrans.first == jTrans.first)
							states[index]->addTransition(iTrans.first, states[iTrans.second->getNumber() * sSize + jTrans.second->getNumber()]);
				}
			}
		}
		DFA res(std::move(states), alphabet);
		return res;
	}

	void DFA::print() {
		int k = 0;
		for (auto i : states) {
			std::cout << "{ " << i->getNumber() << (i->isError() ? " E" : "") << (i->isRecieving() ? " R" : "") << " }"<< std::endl << "transitions:  ";
			for (auto j : i->getTransitions()) {
				std::cout << std::endl << "        ( " << j.first << " " << ")    ->      { " << j.second->getNumber() << (j.second->isError() ? " E" : "") << (j.second->isRecieving() ? " R" : "") << " };" << std::endl;;
			}
			std::cout << std::endl;
		}
	}
}