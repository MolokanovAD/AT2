#include "State.h"

void regexpr::State::addTransition(char a, std::shared_ptr<State> s) {
	transitions.push_back(std::pair<char, SP_State>(a, s));
}
regexpr::SP_State regexpr::State::transmit(char a) {
	for (auto t : transitions) {
		if(t.first == a)
			return t.second;
	}
	return regexpr::SP_State();
}
bool regexpr::State::isError() const {
	for (auto i : transitions) {
		if (i.second.get() != this)
			return false;
	}
	return !recieving;
}
void regexpr::State::addTransitions(const std::vector<std::pair<char, std::shared_ptr<State>>> t) {
	transitions.insert(transitions.end(), t.begin(), t.end());
}