#pragma once
#include "DFA.h"
namespace regexpr {
	std::string intersection(const std::string& first, const std::string& second);
	DFA intersection(const DFA& first, const DFA& second);
	std::string addition(const std::string& expression);
	DFA addition(const DFA& dfa);
	std::string recoverRegularExpression(const DFA& obj);
	DFA compile(const std::string& expression);
	bool match(const std::string& expression, const std::string& s);
}