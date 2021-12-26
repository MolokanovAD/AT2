#pragma once
#include "State.h"
#include <algorithm>
#include <list>
#include <map>
namespace regexpr {
	typedef std::vector<std::vector<std::vector<std::shared_ptr<std::string>>>> vector3d;
	class DFA {
	private:
		std::string expr;
		std::unordered_set<char> alphabet;
		std::shared_ptr<State> startState;
		std::vector<std::shared_ptr<State>> states;
		std::shared_ptr<State> firstNotProcessed();
	public:
		DFA(const std::unordered_set<int>& first, std::vector<std::pair<int, std::unordered_set<int>>> followPos, const std::string& s, std::unordered_set<char> a);
		void minimize();
		void buildBasis(vector3d& matrix, std::set<char> metaSymbols);
		std::string kPath(vector3d& matrix, SP_State iState, SP_State jState, int k);
		std::string k_path(vector3d memoryMatrix, std::unordered_set<std::string>& meta, std::shared_ptr<State> i, std::shared_ptr<State> j, unsigned short k);
		std::string regexRecover();
		void print();
		~DFA() {}
	};
	typedef std::tuple<SP_State, int, int, std::map<char,int>> Split;//<state,previous group, current group, goes to>
}