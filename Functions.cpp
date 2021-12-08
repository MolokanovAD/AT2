#include "Functions.h"

namespace regexpr {
	void addPositions(std::vector<std::pair<int, std::unordered_set<int>>>& followPos, std::unordered_set<int> first, std::unordered_set<int> second) {
		for (auto pos : first) {
			//for each position which is a name of line in FP table
			auto fpLine = followPos.begin();
			for (; fpLine < followPos.end(); fpLine++) {
				if ((*fpLine).first == pos) {
					(*fpLine).second.insert(second.begin(), second.end());
					break;
				}
			}
			if (fpLine == followPos.end()) {
				//if there is no such line in FP table
				followPos.push_back(std::pair<int, std::unordered_set<int>>(pos, second));
			}
		}
	}
}