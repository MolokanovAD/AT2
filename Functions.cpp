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
		int counter = 1;
		int repeat = result.find('{');
		std::unordered_set<char> metaSymbols = {  '#', '+','^','.','|','{','}' };
		while (repeat != -1) {
			int from, to = INT_MAX;
			It i = result.begin() + repeat + 1;
			try {
				//parsing the string
				from = parseInt(i, result.end());
			}
			catch (std::exception& e) {
				throw e;
			}
			if (i < result.end() && *i == ',') {
				//if ',' is met there is a second limit
				//repeat same thing
				i++;
				try {
					to = parseInt(i, result.end());
				}
				catch (std::exception& e) {
					throw e;
				}
			}
			if (i >= result.end() || *i != '}' || from > to)
				throw std::exception("Syntax error");
			if (from == 0 && to == INT_MAX) {
				repeat = result.find('{', i - result.begin());
				continue;
			}
			It repeatEnd = i + 1;
			i = result.begin() + repeat - 1;
			if (metaSymbols.find(*i) != metaSymbols.end()) {
				if (*(i - 1) == '#')
					i--;
				else
					throw std::exception("Syntax error");

			}
			if (*i == ')') {
				while (i >= result.begin() && counter) {
					i--;
					if (*i == ')')
						counter++;
					if (*i == '(')
						counter--;
				}
				if (i < result.begin())
					throw std::exception("Syntax error");
			}
			result.erase(result.begin() + repeat, repeatEnd);
			std::string rep;
			rep.append(result, i - result.begin(), repeat - (i - result.begin()));
			for (int k = from; (k-1) > 0; k--) {
				result.insert(repeat, rep);
			}
			std::string notNeccesary("(^");
			if (to == INT_MAX) {
				result.insert(repeat + (from - 1) * rep.length(), "+");
			}
			else {
				for (int k = 1; k <= (to - from); k++) {
					notNeccesary += '|';
					for (int j = 1; j <= k; j++) {
						notNeccesary.append(rep);
					}
				}
				notNeccesary += ')';
				result.insert(repeat + (from - 1) * rep.length(), notNeccesary);
			}
			repeat = result.find('{',i-result.begin());
		}
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