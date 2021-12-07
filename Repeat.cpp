#include "Repeat.h"
#include "Functions.h"
namespace regexpr {
	void Repeat::buildFollowPos(std::vector<std::pair<int, std::vector<int>>>& followPos) {
		if (to > 1)
			addPositions(followPos, child->lastPositions(), child->firstPositions());
	}
}