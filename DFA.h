#pragma once
#include "State.h"
#include <algorithm>
#include <list>
#include <map>
namespace regexpr {
	enum MODE {
		DIFFERENCE,
		INTERSECTION
	};
	typedef std::vector<std::vector<std::vector<std::shared_ptr<std::string>>>> vector3d;
	class DFA {
	private:
		std::string expr;
		std::unordered_set<char> alphabet;
		std::shared_ptr<State> startState;
		std::vector<std::shared_ptr<State>> states;
		std::shared_ptr<State> firstNotProcessed();
	public:
		DFA(std::vector<std::shared_ptr<State>> s, std::unordered_set<char> a) : alphabet(a), expr("") { states = s; startState = states.front(); }
		DFA(const std::unordered_set<int>& first, std::vector<std::pair<int, std::unordered_set<int>>> followPos, const std::string& s, std::unordered_set<char> a);
		DFA minimize();
		bool match(const std::string& str);
		void buildBasis(vector3d& matrix, std::set<char> metaSymbols) const;
		std::string kPath(vector3d& matrix, SP_State iState, SP_State jState, int k) const;
		std::string recoverExpression() const;
		std::unordered_set<char> getAlphabet()const { return alphabet; }
		friend DFA product(const DFA& first, const DFA& second,MODE mode);
		void print();
		~DFA() {}
	};
	typedef std::tuple<SP_State, int, int, std::map<char,int>> Split;//<state,previous group, current group, goes to>
}