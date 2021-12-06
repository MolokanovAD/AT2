#pragma once
#include "Node.h"
namespace regexpr {
    class LastNode : public Node {
    public:
        LastNode(std::string::const_iterator i) : Node(false, i) {}
        bool isProcessed()override { return false; }
        void print() override {}
        virtual ~LastNode() {}
    };
}
