#include "Functions.h"

namespace regexpr {
	void addPositions(std::vector<std::pair<It, PosVector>>& followPos, PosVector first, PosVector second) {
		for (auto k : first) {
			auto i = followPos.begin();
			for (; i < followPos.end(); i++) {
				if ((*i).first == k) {
					for (auto j : second) {
						if (std::find((*i).second.begin(), (*i).second.end(), j) == (*i).second.end())
							(*i).second.push_back(j);
					}
					continue;
				}
			}
			if (i == followPos.end()) {
				followPos.push_back(std::pair<It, PosVector>(k, second));
			}
		}
	}
}