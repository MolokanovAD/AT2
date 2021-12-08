#pragma once
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_set>

namespace regexpr {

	typedef std::string::const_iterator It;
	//typedef std::vector<std::string::const_iterator> PosVector;
	class Node {
	protected:
		std::shared_ptr<Node> parent;

		bool nullable;
		std::unordered_set<int> firstPos;
		std::unordered_set<int> lastPos;
	public:
		Node() {}
		Node(bool n) :nullable(n) {}
		Node(bool n, std::unordered_set<int> f, std::unordered_set<int> l) :nullable(n),firstPos(f),lastPos(l) {}
		Node(bool n, int l) :nullable(n) { firstPos.insert(l); lastPos.insert(l); }
		void setParent(std::shared_ptr<Node> p) { parent = p; }
		virtual bool isNotProcessed() = 0;
		virtual bool isNullable() { return nullable; }
		std::unordered_set<int> firstPositions() { return firstPos; }
		std::unordered_set<int> lastPositions() { return lastPos; }
		virtual void buildPositions() {}
		virtual void buildFollowPos(std::vector<std::pair<int, std::unordered_set<int>>>& followPos) {  }
		virtual void print() = 0;
		virtual ~Node() {}
	};
	typedef std::shared_ptr<Node> SP_Node;
	typedef std::shared_ptr<std::string::const_iterator> SP_It;
}