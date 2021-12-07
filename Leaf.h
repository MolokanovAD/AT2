#pragma once
#include <string>
#include "Node.h"
namespace regexpr {
	class Leaf : public Node {
	private:
		char value;
	public:
		Leaf() :Node(true) {}
		Leaf(int i, char val) :Node(false,i), value(val) {}
		//bool isEmptyString() const{ return nullable; }
		bool isNotProcessed() override { return false; }
		char getValue() const{ return value; }

		virtual void print();
		virtual ~Leaf() {}
	};
	typedef std::shared_ptr<Leaf> SP_Leaf;
}