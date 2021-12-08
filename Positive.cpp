#include "Positive.h"
#include "Functions.h"
namespace regexpr {
	void Positive::buildFollowPos(std::vector<std::pair<int, std::unordered_set<int>>>& followPos) {
		addPositions(followPos, child->lastPositions(), child->firstPositions());
	}
}