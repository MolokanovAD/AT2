#pragma once
#include <string>
#include "Node.h"
namespace regexpr {
	class Leaf : public Node {
	private:
		char value;
		
	public:
		Leaf() :Node(true) {}
		Leaf(std::string::const_iterator i) :Node(false,i), value(*i) {}
		bool isEmptyString() const{ return nullable; }
		bool isEmpty() override { return false; }
		char getValue() const{ return value; }

		virtual void print();
	};
	typedef std::shared_ptr<Leaf> SP_Leaf;
}