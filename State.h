#pragma once
#include "Node.h"
#include <set>
namespace regexpr {
	class State {
	private:
		bool recieving;
		bool processed;
		std::unordered_set<int> positions;
		//std::vector<std::pair<std::vector<int>,std::shared_ptr<State>>> transitions;
		std::vector<std::pair<char,std::shared_ptr<State>>> transitions;
	public:
		State() {}
		State(std::unordered_set<int> p,bool r):positions(p),recieving(r),processed(false){}
		std::unordered_set<int> getPositions() const { return positions; }
		std::vector<std::pair<char, std::shared_ptr<State>>> getTransitions() { return transitions; }
		bool isProcessed()const { return processed; }
		bool isRecieving()const { return recieving; }
		bool isError()const { return positions.empty(); }
		void process() { processed = true; }
		void addTransition(char a, std::shared_ptr<State> s);
		std::shared_ptr<State> transmit(char a);
	};
	typedef std::shared_ptr<State> SP_State;
}