#include "UnaryOperator.h"
namespace regexpr {
	void UnaryOperator::buildPositions() {
		std::vector<int> res = child->firstPositions();
		firstPos = res;
		res = child->lastPositions();
		lastPos = res;
	}
}