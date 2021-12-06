#pragma once
#include "Node.h"
namespace regexpr {
	void addPositions(std::vector<std::pair<It, PosVector>>& followPos, PosVector first, PosVector second);
}