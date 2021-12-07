#pragma once
#include "Node.h"
namespace regexpr {
    class LastNode : public Node {
    public:
        LastNode(int i) : Node(false, i) {}
        bool isNotProcessed()override { return false; }
        void print() override {}
        virtual ~LastNode() {}
    };
}
