#pragma once
#include "Node.h"
namespace regexpr {
	void addPositions(std::vector<std::pair<int, std::unordered_set<int>>>& followPos, std::unordered_set<int> first, std::unordered_set<int> second);
	std::string wrapWithBrackets(const std::string& source);
	std::string KleeneClosure(const std::string& source);
	std::string addCondition(const std::string& source, const std::string& condition);
}