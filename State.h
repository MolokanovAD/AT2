#pragma once
#include "Node.h"
namespace regexpr {
	class State {
	private:
		bool recieving;
		PosVector positions;
	public:
		State(PosVector p,bool r):positions(p),recieving(r){}
		PosVector getPositions() const { return positions; }
	};
}