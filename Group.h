#pragma once
#include "UnaryOperator.h"
namespace regexpr {
    class Group : public UnaryOperator {
    private:
        int number;
    public:
        Group() {}
        Group(SP_Node child, int n) :UnaryOperator(child), number(n) {}
        bool isNullable() override { return child->isNullable(); }
        virtual void print() override { std::cout << "g"<< number << ":"; }
    };
    typedef std::shared_ptr<Group> SP_G;
}
