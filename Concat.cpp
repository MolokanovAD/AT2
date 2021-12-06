#include "Concat.h"
#include "Functions.h"
namespace regexpr {
	void Concat::buildPositions() {
		if (leftChild->isNullable()) {
			PosVector res = leftChild->firstPositions();
			PosVector right = rightChild->firstPositions();
			res.insert(res.end(), std::make_move_iterator(right.begin()), std::make_move_iterator(right.end()));
			firstPos = res;
			return;
		}
		firstPos = leftChild->firstPositions();
		if (rightChild->isNullable()) {
			PosVector res = leftChild->lastPositions();
			PosVector right = rightChild->lastPositions();
			res.insert(res.end(), std::make_move_iterator(right.begin()), std::make_move_iterator(right.end()));
			lastPos = res;
			return;
		}
		lastPos = rightChild->lastPositions();
	}
	void Concat::buildFollowPos(std::vector<std::pair<It, PosVector>>& followPos) {
		addPositions(followPos, leftChild->lastPositions(), rightChild->firstPositions());
	}
}