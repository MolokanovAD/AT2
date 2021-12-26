#include "DFA.h"
#include "Functions.h"
regexpr::DFA::DFA(const std::unordered_set<int>& first, std::vector<std::pair<int, std::unordered_set<int>>> followPos, const std::string& s, std::unordered_set<char> a):expr(s),alphabet(a) {
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
				states.push_back(std::make_shared<State>(newStatePositions, newStatePositions.find(expr.length()) != newStatePositions.end(),n));
				n++;
				S->addTransition(c, *(--states.end()));
			}
		}
		S->process();
		S = firstNotProcessed();
	}
	std::sort(states.begin(), states.end(), [](SP_State a, SP_State b) {return a->getNumber() < b->getNumber(); });
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
		bool isStart = false;
		Split s;
		for (auto& i : oldStates) {
			if (std::get<2>(i) == g) {
				isStart = std::get<0>(i).get() == startState.get();
				s = i;
				for (auto p : std::get<0>(i)->getPositions())
					pos.insert(p);
				rec = std::get<0>(i)->isRecieving();
			}
		}
		states.push_back(std::make_shared<State>(pos, rec, g));
		if (isStart)
			startState = states.back();
		//states.push_back(std::make_shared<State>(pos, rec, g + 1));
		transit.push_back(std::get<3>(s));
	}
	for (char c : alphabet) {
		auto j = transit.begin();
		for (auto i = states.begin(); i < states.end();i++,j++) {
			(*i)->addTransition(c, states[(*j)[c]]);
		}
	}
	std::sort(states.begin(), states.end(), [](SP_State a, SP_State b) {return a->getNumber() < b->getNumber(); });
}
std::shared_ptr<regexpr::State> regexpr::DFA::firstNotProcessed() {
	auto i = states.begin();
	while (i < states.end() && (*i)->isProcessed()) {
		i++;
	}
	return i == states.end() ? SP_State(nullptr) : *i;
}
std::string regexpr::DFA::regexRecover() {
	vector3d matrix(states.size(),std::vector<std::vector<std::shared_ptr<std::string>>>(states.size(), std::vector<std::shared_ptr<std::string>>(states.size(),nullptr)));
	std::set<char> metaSymbols;
	metaSymbols.insert(')');
	metaSymbols.insert('(');
	metaSymbols.insert('#');
	metaSymbols.insert('+');
	metaSymbols.insert('^');
	metaSymbols.insert('.');
	metaSymbols.insert('|');
	metaSymbols.insert('{');
	metaSymbols.insert('}');
	std::string result = "";
	buildBasis(matrix, metaSymbols);
	for (auto state : states) {
		if (state->isRecieving()) {
			if (!result.empty())
				result += "|";
			result += kPath(matrix, startState, state, states.size() - 1);
		}
	}
	return result;
}

void regexpr::DFA::buildBasis(vector3d& matrix, std::set<char> metaSymbols) {
	for (auto state : states) {
		int i = state->getNumber();
		for (auto& transition : state->getTransitions()) {
			int j = transition.second->getNumber();
			std::string sign("");
			sign += transition.first;
			if (metaSymbols.find(transition.first) != metaSymbols.end())
				sign = "#" + transition.first;
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
		if(!matrix[i][i][0].get())
			matrix[i][i][0] = std::make_shared<std::string>("^");
		else {
			matrix[i][i][0]->insert(matrix[i][i][0]->begin(), '(');
			*matrix[i][i][0] += "|^)";
		}
	}
}

std::string regexpr::DFA::kPath(vector3d& matrix, SP_State iState, SP_State jState, int k) {
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
			if (repeat.front() == '(')
				firstPart += repeat + "{0}";
			else
				firstPart += "(" + repeat + "){0}";
		}
		if (!result.empty()) {
			if (result.front() == '(') 
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

std::string regexpr::DFA::k_path(vector3d memoryMatrix, std::unordered_set<std::string>& meta, std::shared_ptr<regexpr::State> i, std::shared_ptr<regexpr::State> j, unsigned short k) {
	unsigned short in = i->getNumber(), jn = j->getNumber();
	if (k == 0) {
		if (!memoryMatrix[in][jn][k].get()) {
			if (in == jn)
				memoryMatrix[in][jn][k] = std::make_shared<std::string>("e{,0})");
			else
				memoryMatrix[in][jn][k] = std::make_shared<std::string>("");
			std::vector<std::pair<char, SP_State>> lst = i->getTransitions();
			//std::unique_ptr<std::list<std::unique_ptr<Transition>>> lst(i->getAllTransitions());
			for (auto n = lst.begin(); n < lst.end(); n++) {
				if((*n).second->transmit((*n).first) == j) {
				//if ((*n)->getTargetState() == j) {
					if (!(memoryMatrix[in][jn][k].get() == nullptr)) {//maybe also "" 
						*memoryMatrix[in][jn][k] += '|';
					}
					bool addbr = memoryMatrix[in][jn][k].get() != nullptr && (*memoryMatrix[in][jn][k]).length() > 1;
					if (addbr)
						*memoryMatrix[in][jn][k] += '(';
					std::string tmp("");
					tmp += (*n).first;
					if (meta.find(tmp) != meta.end())
						tmp = '&' + tmp;
					*memoryMatrix[in][jn][k] += tmp.empty() ? "." : tmp;
					if (addbr)
						*memoryMatrix[in][jn][k] += ')';
				}
			}
		}
		return *memoryMatrix[in][jn][k];
	}
	else {
		std::string result = "";
		std::string tmpres = "";
		std::string tmpres2 = "";
		memoryMatrix[in][jn][k - 1] = memoryMatrix[in][jn][k - 1].get() ? memoryMatrix[in][jn][k - 1] : std::make_shared<std::string>(k_path(memoryMatrix, meta, i, j, k - 1));
		tmpres = *memoryMatrix[in][jn][k - 1];
		if (!tmpres.empty()) {
			bool addbr = tmpres.size() > 1;
			if (addbr)
				result += '(';
			result += tmpres;
			if (addbr)
				result += ')';
		}
		memoryMatrix[in][k - 1][k - 1] = memoryMatrix[in][k - 1][k - 1].get() ? memoryMatrix[in][k - 1][k - 1] : std::make_shared<std::string>(k_path(memoryMatrix, meta, i, states[k - 1], k - 1));
		tmpres = *memoryMatrix[in][k - 1][k - 1];
		if (tmpres.empty())
			return result;
		tmpres2 += tmpres;
		tmpres2 += '(';
		memoryMatrix[k - 1][k - 1][k - 1] = memoryMatrix[k - 1][k - 1][k - 1].get() ? memoryMatrix[k - 1][k - 1][k - 1] : std::make_shared<std::string>(k_path(memoryMatrix, meta, states[k - 1], states[k - 1], k - 1));
		tmpres = *memoryMatrix[k - 1][k - 1][k - 1];
		tmpres2 += tmpres;
		tmpres2 += "){,}";
		memoryMatrix[k - 1][jn][k - 1] = memoryMatrix[k - 1][jn][k - 1].get() ? memoryMatrix[k - 1][jn][k - 1] : std::make_shared<std::string>(k_path(memoryMatrix, meta, states[k - 1], j, k - 1));
		tmpres = *memoryMatrix[k - 1][jn][k - 1];
		if (tmpres.empty())
			return result;
		tmpres2 += tmpres;
		bool addbracket = false;
		if (!result.empty()) {
			result += "|";
			if (tmpres2.size() > 1) {
				result += '(';
				addbracket = true;
			}
		}
		result += tmpres2;
		if (addbracket)
			result += ')';
		return result;
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