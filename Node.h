#pragma once
#include <memory>
#include <vector>
#include <string>
#include <iostream>

namespace regexpr {
	enum TYPE {
		OR,
		CONCAT,
		POSITIVE,
		REPEAT,
		GROUP,
		BRACKETS,
		OPEN_BRACKET,
		CLOSE_BRACKET,
		SCREEN,
	};

	typedef std::string::const_iterator It;
	typedef std::vector<std::string::const_iterator> PosVector;
	class Node {
	protected:
		std::shared_ptr<Node> parent;

		bool nullable;
		PosVector firstPos;
		PosVector lastPos;
	public:
		Node() {}
		Node(bool n) :nullable(n) {}
		Node(bool n, PosVector f, PosVector l) :nullable(n),firstPos(f),lastPos(l) {}
		Node(bool n, It l) :nullable(n) { firstPos.push_back(l); lastPos.push_back(l); }
		void setParent(std::shared_ptr<Node> p) { parent = p; }
		virtual bool isEmpty() = 0;
		virtual bool isNullable() { return nullable; }
		PosVector firstPositions() { return firstPos; }
		PosVector lastPositions() { return lastPos; }
		virtual void buildPositions() {}
		virtual void buildFollowPos(std::vector<std::pair<It, PosVector>>& followPos) {  }
		virtual void print() = 0;
		virtual ~Node() {}
	};
	typedef std::shared_ptr<Node> SP_Node;
	typedef std::shared_ptr<std::string::const_iterator> SP_It;
}