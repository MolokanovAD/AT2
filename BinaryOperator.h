#pragma once
#include "Operator.h"
namespace regexpr {
    class BinaryOperator : public Operator {
    protected:
        SP_Node leftChild;
        SP_Node rightChild;
    public:
        BinaryOperator() {}
        BinaryOperator(SP_Node left, SP_Node right) :leftChild(left), rightChild(right) {}
        bool isNotProcessed() override { return !(leftChild.get() || rightChild.get()); }
        void setChildren(SP_Node left, SP_Node right);
        SP_Node getLeftChild() { return leftChild; }
        SP_Node getRightChild() { return rightChild; }

        virtual ~BinaryOperator() {}
    };

}