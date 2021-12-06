#pragma once
#include "UnaryOperator.h"
namespace regexpr {
    class Screen :public UnaryOperator {
    private:
        char value;
    public:
        Screen(char v):value(v) {}
        bool isNullable() override { return false; };
        virtual void print() override { std::cout << "#"; }
    };

}