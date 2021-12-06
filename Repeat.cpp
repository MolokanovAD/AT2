#include "Repeat.h"
#include "Functions.h"
namespace regexpr {
	void Repeat::buildFollowPos(std::vector<std::pair<It, PosVector>>& followPos) {
		if (to > 1)
			addPositions(followPos, child->Node::lastPositions(), child->Node::firstPositions());
	}
}