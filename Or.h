#pragma once
#include "BinaryOperator.h"
namespace regexpr {
    class Or : public BinaryOperator {
    public:
        Or() {}
        void buildPositions() override ;
        bool isNullable() override { return leftChild->isNullable() || rightChild->isNullable(); };
        virtual void print() override { std::cout << "|"; }
    };
}
