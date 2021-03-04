#ifndef NODE_H
#define NODE_H
#include "Global.h"

template <class C>
class Node {
private:
	C data;
	Node<C>*	link;
public:
	Node() {}
	~Node() {}

	void SetData(C _data) { data = _data; }
	void Setlink(Node<C>* _link) { link = _link; }

	C GetData() { return data; }
	Node<C>* Link() { return link; }
};

#endif