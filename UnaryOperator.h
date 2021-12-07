#pragma once
#include "Operator.h"
namespace regexpr {
	class UnaryOperator : public Operator {
	protected:
		SP_Node child;
	public:
		UnaryOperator() : Operator() {}
		UnaryOperator(SP_Node c) : Operator(),child(c) {}
		bool isNotProcessed() override { return !child.get(); }
		void setChild(SP_Node c) { child = c; }
		SP_Node getChild() { return child; }

		void buildPositions() override;
		virtual ~UnaryOperator() {}
	};
}