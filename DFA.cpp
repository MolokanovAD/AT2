#include "DFA.h"
regexpr::DFA::DFA(const PosVector& first, std::vector<std::pair<It, PosVector>> followPos, const std::string& s) {
	followPos.back().second.back() == s.end();
	//state.push_back(std::make_shared<State>(first, std::find(first.begin(), first.end(), last) == first.end()));
}