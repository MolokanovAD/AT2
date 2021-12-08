#include "Concat.h"
#include "Functions.h"
namespace regexpr {
	void Concat::buildPositions() {
		if (leftChild->isNullable()) {
			std::unordered_set<int> res = leftChild->firstPositions();
			std::unordered_set<int> right = rightChild->firstPositions();
			res.insert(std::make_move_iterator(right.begin()), std::make_move_iterator(right.end()));
			firstPos = res;
			return;
		}
		firstPos = leftChild->firstPositions();
		if (rightChild->isNullable()) {
			std::unordered_set<int> res = leftChild->lastPositions();
			std::unordered_set<int> right = rightChild->lastPositions();
			res.insert(std::make_move_iterator(right.begin()), std::make_move_iterator(right.end()));
			lastPos = res;
			return;
		}
		lastPos = rightChild->lastPositions();
	}
	void Concat::buildFollowPos(std::vector<std::pair<int, std::unordered_set<int>>>& followPos) {
		addPositions(followPos, leftChild->lastPositions(), rightChild->firstPositions());
	}
}