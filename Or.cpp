#include "Or.h"
namespace regexpr {
	void Or::buildPositions() {
		PosVector res = leftChild->firstPositions();
		PosVector right = rightChild->firstPositions();
		res.insert(res.end(), std::make_move_iterator(right.begin()), std::make_move_iterator(right.end()));
		firstPos = res;
		res = leftChild->lastPositions();
		right = rightChild->lastPositions();
		res.insert(res.end(), std::make_move_iterator(right.begin()), std::make_move_iterator(right.end()));
		lastPos = res;
	}
}