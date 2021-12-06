#pragma once
#include "State.h"
#include <list>
namespace regexpr {
	class DFA {
	private:
		std::string expr;
		std::list<std::shared_ptr<State>> state;
	public:
		DFA(const PosVector& first, std::vector<std::pair<It, PosVector>> followPos, const std::string& s);
	};

}