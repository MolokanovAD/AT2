#include "Concat.h"
#include "Functions.h"
namespace regexpr {
	void Concat::buildPositions() {
		if (leftChild->isNullable()) {
			std::vector<int> res = leftChild->firstPositions();
			std::vector<int> right = rightChild->firstPositions();
			res.insert(res.end(), std::make_move_iterator(right.begin()), std::make_move_iterator(right.end()));
			firstPos = res;
			return;
		}
		firstPos = leftChild->firstPositions();
		if (rightChild->isNullable()) {
			std::vector<int> res = leftChild->lastPositions();
			std::vector<int> right = rightChild->lastPositions();
			res.insert(res.end(), std::make_move_iterator(right.begin()), std::make_move_iterator(right.end()));
			lastPos = res;
			return;
		}
		lastPos = rightChild->lastPositions();
	}
	void Concat::buildFollowPos(std::vector<std::pair<int, std::vector<int>>>& followPos) {
		addPositions(followPos, leftChild->lastPositions(), rightChild->firstPositions());
	}
}