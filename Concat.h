#pragma once
#include "BinaryOperator.h"
namespace regexpr {
	class Concat :public BinaryOperator {
	public:
		Concat() {}
		Concat(SP_Node left,SP_Node right):BinaryOperator(left,right) {}

		void buildPositions() override ;
		bool isNullable() override { return leftChild->isNullable() && rightChild->isNullable(); };
		void buildFollowPos(std::vector<std::pair<int, std::unordered_set<int>>>& followPos) override;

		virtual void print() override { std::cout << "."; }

	};

}