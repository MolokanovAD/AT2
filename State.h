#pragma once
#include "Node.h"
namespace regexpr {
	class State {
	private:
		bool recieving;
		bool processed;
		std::vector<int> positions;
		std::vector<std::pair<std::vector<int>,std::shared_ptr<State>>> transitions;
	public:
		State(std::vector<int> p,bool r):positions(p),recieving(r),processed(false){}
		std::vector<int> getPositions() const { return positions; }
		std::vector<std::pair<std::vector<int>, std::shared_ptr<State>>> getTransitions() { return transitions; }
		bool isNotProcessed()const { return processed; }
		bool isRecieving()const { return recieving; }
		void process() { processed = true; }
		bool samePositions(std::vector<int> v)const;
		void addTransition(std::vector<int> a, std::shared_ptr<State> s);
	};
}