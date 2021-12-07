#include "State.h"

bool regexpr::State::samePositions(std::vector<int> v) const {
	if(v.size() != positions.size())
		return false;
	for (auto i : v)
		if (std::find(positions.begin(), positions.end(), i) == positions.end())
			return false;
	return true;
}

void regexpr::State::addTransition(std::vector<int> a, std::shared_ptr<State> s) {
	transitions.push_back(std::pair<std::vector<int>, std::shared_ptr<State>>(a, s));
}
