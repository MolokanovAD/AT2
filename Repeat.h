#pragma once
#include "UnaryOperator.h"
namespace regexpr {
    class Repeat :public UnaryOperator {
    private:
        int from;
        int to;
    public:
        Repeat(int a):from(a),to(INT_MAX){}
        Repeat(int a,int b) :from(a), to(b) {}

        void buildFollowPos(std::vector<std::pair<int, std::unordered_set<int>>>& followPos) override;
        bool isNullable() override { return child->isNullable() || from == 0; }
        std::pair<int, int> getLimits() { return std::pair<int, int>(from, to); }

        virtual void print() override { std::cout << "rep{" << from << "," << to << "}"; }
    };
    typedef std::shared_ptr<Repeat> SP_Rep;
}