#pragma once

#include "Global.h"

template <typename c>
class Node {
private:
	c data;
	Node<c>* link;
public:
	Node() {
		memset(&data, 0, sizeof(data));
		link = nullptr;
	}
	~Node() {}

	Node<c>* Getlink() { return link; }
	c GetData() { return data; }

	void Setlink(Node<c>* _link) { link = _link; }
	void Setdata(c _data) { data = _data; }
};