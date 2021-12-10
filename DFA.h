#pragma once
#include "State.h"
#include <algorithm>
#include <list>
#include <map>
namespace regexpr {
	class DFA {
	private:
		std::string expr;
		std::unordered_set<char> alphabet;
		std::vector<std::shared_ptr<State>> states;
		std::shared_ptr<State> firstNotProcessed();
	public:
		DFA(const std::unordered_set<int>& first, std::vector<std::pair<int, std::unordered_set<int>>> followPos, const std::string& s, std::unordered_set<char> a);
		void minimize();
		void print();
		~DFA() {}
	};
	typedef std::tuple<SP_State, int, int, std::map<char,int>> Split;//<state,previous group, current group, goes to>
}