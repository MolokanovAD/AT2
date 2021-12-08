#include "Or.h"
namespace regexpr {
	void Or::buildPositions() {
		std::unordered_set<int> res = leftChild->firstPositions();
		std::unordered_set<int> right = rightChild->firstPositions();
		res.insert( std::make_move_iterator(right.begin()), std::make_move_iterator(right.end()));
		firstPos = res;
		res = leftChild->lastPositions();
		right = rightChild->lastPositions();
		res.insert(std::make_move_iterator(right.begin()), std::make_move_iterator(right.end()));
		lastPos = res;
	}
}