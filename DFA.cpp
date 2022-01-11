#include "DFA.h"
#include "Functions.h"
namespace regexpr {
	DFA::DFA(const std::unordered_set<int>& first, std::vector<std::pair<int, std::unordered_set<int>>> followPos, const std::string& s, std::unordered_set<char> a) :expr(s), alphabet(a) {
		states.push_back(std::make_shared<State>(first, std::find(first.begin(), first.end(), expr.length()) != first.end(), 0));
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
	}

	DFA DFA::minimize() {
		std::vector<Split> oldStates;
		int groupsTotal = 1;
		if (states.size() == 1)
			return *this;
		bool isStartRec = states.front()->isRecieving();
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
		return *this;
	}

	bool regexpr::DFA::match(const std::string& str) {
		auto state = states.front();
		for (char s : str) {
			if (alphabet.find(s) == alphabet.end())
				return false;
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
		vector3d matrix(states.size(), std::vector<std::vector<std::shared_ptr<std::string>>>(states.size(), std::vector<std::shared_ptr<std::string>>(states.size() + 1, nullptr)));
		std::set<char> metaSymbols = { ')','(', '#', '+','^','.','|','{','}' };
		std::string result = "", newPath = "";
		buildBasis(matrix, metaSymbols);
		for (auto state : states) {
			if (state->isRecieving()) {
				newPath = kPath(matrix, states.front(), state, states.size());
				if (!result.empty() && !newPath.empty())
					result += "|";
				result += newPath;
				newPath.clear();
				
			}
		}
		return result;
	}

	void DFA::buildBasis(vector3d& matrix, std::set<char> metaSymbols) const{
		for (auto& state : states) {
			int i = state->getNumber();
			for (auto& transition : state->getTransitions()) {
				int j = transition.second->getNumber();
				std::string sign("");
				sign += transition.first;
				if (metaSymbols.find(transition.first) != metaSymbols.end())
					sign.insert(sign.begin(), '#');
				if (!matrix[i][j][0].get())
					matrix[i][j][0] = std::make_shared<std::string>(sign);
				else
					matrix[i][j][0] = std::make_shared<std::string>(addCondition((*matrix[i][j][0]), sign));
			}
			if (!matrix[i][i][0].get())
				matrix[i][i][0] = std::make_shared<std::string>("^");
			else
				matrix[i][i][0] = std::make_shared<std::string>(addCondition((*matrix[i][i][0]), "^"));
		}
	}

	std::string DFA::kPath(vector3d& matrix, SP_State iState, SP_State jState, int k) const{
		int i = iState->getNumber();
		int j = jState->getNumber();
		if (k == 0)
			return matrix[i][j][0].get() ? *matrix[i][j][0] : "";
		if (matrix[i][j][k].get())
			return *matrix[i][j][k];
		std::string result = matrix[i][j][k - 1].get() ? *matrix[i][j][k - 1] : kPath(matrix, iState, jState, k - 1);
		std::string firstPart = matrix[i][k - 1][k - 1].get() ? *matrix[i][k - 1][k - 1] : kPath(matrix, iState, states[k - 1], k - 1);
		std::string lastPart = matrix[k - 1][j][k - 1].get() ? *matrix[k - 1][j][k - 1] : kPath(matrix, states[k - 1], jState, k - 1);
		if (!firstPart.empty() && !lastPart.empty()) {
			std::string repeat = matrix[k - 1][k - 1][k - 1].get() ? *matrix[k - 1][k - 1][k - 1] : kPath(matrix, states[k - 1], states[k - 1], k - 1);
			if (!repeat.empty() && repeat != "^") {
				if (repeat == firstPart)
					firstPart = wrapWithBrackets(firstPart) + "+";//KleeneClosure(firstPart);
				else
					firstPart += KleeneClosure(repeat);
			}
			result = addCondition(result, firstPart + lastPart);
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
		SP_State errorState;
		for (auto& i : first.states) {
			for (auto& j : second.states) {
				int index = i->getNumber() * sSize + j->getNumber();//индекс вершины в массиве, которая отвечает за комбинацию (i,j) вершин исходных автоматов
				if (mode == DIFFERENCE && i->isRecieving() && !j->isRecieving())
					states[index]->setRecieving(true);
				else if(mode == INTERSECTION && i->isRecieving() && j->isRecieving())
					states[index]->setRecieving(true);
				for (auto& iTrans : i->getTransitions()) {
					for (auto& jTrans : j->getTransitions())
						if(iTrans.first == jTrans.first)//если найден переход в первом и втором автомате по одному и тому же символу
							states[index]->addTransition(iTrans.first, states[iTrans.second->getNumber() * sSize + jTrans.second->getNumber()]);//создаем в вершине (i,j) переход в вершину, отвечающую за те, в которые ведут переходы по символу
				}
				if (states[index]->isError())//если состояние является ошибочным, запоминаем его в переменной errorState
					errorState = states[index];
			}
		}
		if (first.alphabet != second.alphabet) {//если алфавиты не совпадают, то переходов еще недостаточно, тк вверху делались переходы только по тем символам, которые есть в обоих алфавитах (208 строчка)
			if (!errorState.get()) {//если в прошлом цикле в errorState так ничего и не записали, значит состояния ошибки в автомате еще нет, надо создать
				errorState = std::make_shared<State>(states.size());
				for (auto& t : states[0]->getTransitions())//добавляем переходы из состояния ошибки в само себя только по тем символам, которые есть в обоих алфавитах, тк с остальными будем разбираться дальше
					errorState->addTransition(t.first, errorState);
				states.push_back(errorState);//добавляем errorState в массив состояний
			}
			std::vector<std::pair<char, SP_State>> t;//создаем массив переходов по символам, которых нет в одном из алфавитов, он будет общим для всех вершин, потому что все переходы в нем идут в errorState
			for (char c : alphabet) {//идем по всем символам результирующего алфавита(объединение исходных)
				if (first.alphabet.find(c) == first.alphabet.end() || second.alphabet.find(c) == second.alphabet.end())//если символ из рез. алфавита отсутствует хотя бы в одном из исходных алфавитов, переход из любой вершины автомата по этому символу будет вести в состояние ошибки
					t.push_back(std::pair<char, SP_State>(c, errorState));//добавляем переход в errorState по этому символу
			}
			for (auto& state : states) {//теперь в каждую вершину добавляем полученные переходы в состояние ошибки
				state->addTransitions(t);
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
