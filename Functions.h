#pragma once
#include "Node.h"
namespace regexpr {
	void addPositions(std::vector<std::pair<int, std::vector<int>>>& followPos, std::vector<int> first, std::vector<int> second);
}