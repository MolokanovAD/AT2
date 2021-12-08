#include "State.h"

void regexpr::State::addTransition(char a, SP_State s) {
	transitions.push_back(std::pair<char, SP_State>(a, s));
}
