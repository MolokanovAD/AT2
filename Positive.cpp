#include "Positive.h"
#include "Functions.h"
namespace regexpr {
	void Positive::buildFollowPos(std::vector<std::pair<int, std::vector<int>>>& followPos) {
		addPositions(followPos, child->lastPositions(), child->firstPositions());
	}
}