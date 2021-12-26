#pragma once
#include "Node.h"
#include <set>
namespace regexpr {
	class State {
	private:
		bool recieving;
		bool processed;
		std::unordered_set<int> positions;
		int num;
		std::vector<std::pair<char,std::shared_ptr<State>>> transitions;
	public:
		State() {}
		State(std::unordered_set<int> p,bool r,int n):positions(p),recieving(r),processed(false),num(n){}
		std::unordered_set<int> getPositions() const { return positions; }
		bool isProcessed()const { return processed; }
		bool isRecieving()const { return recieving; }
		bool isError()const { return positions.empty(); }
		int getNumber() { return num; }
		std::vector<std::pair<char, std::shared_ptr<State>>> getTransitions() { return transitions; }
		void process() { processed = true; }
		void addTransition(char a, std::shared_ptr<State> s);
		std::shared_ptr<State> transmit(char a);
	};
	typedef std::shared_ptr<State> SP_State;
}