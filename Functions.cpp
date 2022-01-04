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
	It findBracket(const std::string& source, It it) {
		int counter = 1;
		It i = it;
		if (*i == ')' && (i == source.begin() || *(i - 1) != '#')) {
			do {
				i--;
				if (*i == ')' && (i == source.begin() || *(i-1) != '#'))
					counter++;
				if (*i == '(' && (i == source.begin() || *(i - 1) != '#'))
					counter--;
			} while (i != source.begin() && counter);
			if (i == source.begin() && counter)
				throw std::exception("Syntax error");
			return i;
		}
		return source.end();
	}
	std::string wrapWithBrackets(const std::string& source) {
		if(findBracket(source, --source.end()) == source.begin() || source.length() == 1)
			return source;
		return "(" + source + ")";
	}
	std::string KleeneClosure(const std::string& source) {
		if (source.length() > 3) {
			std::string Check(source.end() - 4, source.end() - 1);
			if (Check == "){0}" && source.front() == '(')// (...){0} case
				return source;
			Check.erase(Check.begin());
			if (Check == "{0}" && source.length() == 4)// _{0} case
				return source;
		}
		return wrapWithBrackets(source) + "{0}";
	}
	bool regexpr::isEqual(const std::string& first, const std::string& second) {
		if (first == "^|" + second)
			return true;
		return false;
	}
	std::string addCondition(const std::string& source, const std::string& condition) {
		std::string result = source;
		if (!result.empty()) {
			if (findBracket(result, --result.end()) == result.begin()) {
				result.erase(result.length() - 1);
			}
			/*if (result.back() == ')')
				result.erase(result.length() - 1);*/
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
		std::unordered_set<char> metaSymbols = {  '#', '+','^','.','|','{','}' };
		for (auto i = result.begin(); i < result.end(); i++) {
			if (*i == '+') {
				if (i == result.begin() || (metaSymbols.find(*(i - 1)) != metaSymbols.end() && ((i - 1) == result.begin() || *(i - 2) != '#')))
					throw std::exception("Syntax error");
			}
		}
		int counter = 1;
		int repeat = result.find('{');
		while (repeat != -1) {
			int from, to = INT_MAX;
			It i = result.begin() + repeat + 1;
			try {
				from = parseInt(i, result.end());
			}
			catch (std::exception& e) {
				throw e;
			}
			if (i < result.end() && *i == ',') {
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
			/*if (*i == ')' && (i == result.begin() || *(i - 1) != '#')) {
				while (i >= result.begin() && counter) {
					i--;
					if (*i == ')' && (i == result.begin() || *(i - 1) != '#'))
						counter++;
					if (*i == '(' && (i == result.begin() || *(i - 1) != '#'))
						counter--;
				}
				if (i < result.begin())
					throw std::exception("Syntax error");
			}*/
			auto it = findBracket(result, i);
			i = it != result.end() ? it : i;
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
			repeat = result.find('{');
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