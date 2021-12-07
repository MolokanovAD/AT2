#pragma once
#include "State.h"
#include <algorithm>
#include <list>
namespace regexpr {
	class DFA {
	private:
		std::string expr;
		std::vector<std::shared_ptr<State>> states;
	public:
		DFA(const std::vector<int>& first, std::vector<std::pair<int, std::vector<int>>> followPos, const std::string& s);
		void print();
		~DFA() {}
	};

}