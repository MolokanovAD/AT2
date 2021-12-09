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