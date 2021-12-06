#include "Positive.h"
#include "Functions.h"
namespace regexpr {
	void Positive::buildFollowPos(std::vector<std::pair<It, PosVector>>& followPos) {
		addPositions(followPos, child->lastPositions(), child->Node::firstPositions());
	}
}