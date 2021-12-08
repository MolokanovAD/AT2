#include "UnaryOperator.h"
namespace regexpr {
	void UnaryOperator::buildPositions() {
		firstPos = child->firstPositions();
		lastPos = child->lastPositions();
	}
}