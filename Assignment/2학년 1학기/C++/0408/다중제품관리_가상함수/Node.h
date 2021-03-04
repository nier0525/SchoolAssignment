#pragma once

#include "Global.h"

template <typename c>
class Node {
private:
	c data;
	Node<c>* link;
public:
	Node<c>() { link = nullptr; }
	~Node<c>() {}

	void Setdata(c _data) { data = _data; }
	void SetLink(Node<c>* _link) { link = _link; }

	c Getdata() { return data; }
	Node<c>* Getlink() { return link; }
};