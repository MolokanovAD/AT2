#pragma once
#include "State.h"
#include <algorithm>
#include <list>
namespace regexpr {
	class DFA {
	private:
		std::string expr;
		std::unordered_set<char> alphabet;
		std::vector<std::shared_ptr<State>> states;
		std::shared_ptr<State> firstNotProcessed();
	public:
		DFA(const std::unordered_set<int>& first, std::vector<std::pair<int, std::unordered_set<int>>> followPos, const std::string& s, std::unordered_set<char> a);
		void print();
		~DFA() {}
	};

}