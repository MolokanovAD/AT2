#pragma once
#include "Node.h"
#include "UnaryOperator.h"
#include "ZeroOperator.h"
namespace regexpr {
	//class Brackets : public UnaryOperator {
	//
	//public:
	//	Brackets() {};
	//};
	class Bracket : public ZeroOperator {
		bool isEmpty() override { return true; }
	};
	class OpenBracket : public Bracket {
	public:
		OpenBracket() {}
		virtual void print() override { std::cout << "("; }
	};
	class CloseBracket : public Bracket {
	public:
		CloseBracket() {}
		virtual void print() override { std::cout << ")"; }
	};
	typedef std::shared_ptr<OpenBracket> SP_OB;
	typedef std::shared_ptr<CloseBracket> SP_CB;
}