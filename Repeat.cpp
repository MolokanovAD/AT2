#include "Repeat.h"
#include "Functions.h"
namespace regexpr {
	void Repeat::buildFollowPos(std::vector<std::pair<int, std::unordered_set<int>>>& followPos) {
		if (to > 1)
			addPositions(followPos, child->lastPositions(), child->firstPositions());
	}
}