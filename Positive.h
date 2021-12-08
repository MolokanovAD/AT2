#pragma once
#include "UnaryOperator.h"
namespace regexpr {
    class Positive : public UnaryOperator {
    public:
        Positive() {}

        void buildFollowPos(std::vector<std::pair<int, std::unordered_set<int>>>& followPos) override;
        bool isNullable() override { return child->isNullable(); }

        virtual void print() override { std::cout << "+"; }
    };
    typedef std::shared_ptr<Positive> SP_Pos;
}