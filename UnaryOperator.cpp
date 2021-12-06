#include "UnaryOperator.h"
namespace regexpr {
	void UnaryOperator::buildPositions() {
		PosVector res = child->firstPositions();
		firstPos = res;
		res = child->lastPositions();
		lastPos = res;
	}
}