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
	std::string wrapWithBrackets(const std::string& source) {
		if (source.back() != ')' && source.length() > 1)
			return "(" + source + ")";
		return source;
	}
	std::string KleeneClosure(const std::string& source) {
		if (source.length() > 3) {
			std::string KleeneCheck(source.end() - 4, source.end() - 1);
			if (KleeneCheck == "){0}" && source.front() == '(')// (...){0} case
				return source;
			KleeneCheck.erase(KleeneCheck.begin());
			if (KleeneCheck == "{0}" && source.length() == 4)// _{0} case
				return source;
		}
		return wrapWithBrackets(source) + "{0}";
	}
	std::string addCondition(const std::string& source, const std::string& condition) {
		std::string result = source;
		if (!result.empty()) {
			if (result.back() == ')')
				result.erase(result.length() - 1);
			else
				result.insert(result.begin(), '(');
			result += "|" + condition + ")";
		}
		else
			result += condition;
		return result;
	}
	std::string openRepeats(const std::string& source) {
		std::string result(source);
		It start = result.begin();
		int counter = 0;
		int repeat = result.find('{');
		while (repeat != result.length()) {
			

			repeat = result.find('{');
		}
		/*for (It i = ++source.begin(); i < source.end(); i++) {
			if (*i == '{' && *(i - 1) != '#') {
				result.insert(result.end(), start, (i - 1));

			}
		}*/
		return result;
	}
	int parseInt(It& i, It end) {
		int result = 0;
		std::string str;
		while (i < end && std::isdigit(*i)) {
			str += *i;
			i++;
		}
		try {
			result = std::stoi(str);
		}
		catch (std::exception& e) {
			throw e;
		}
		return result;
	}
}