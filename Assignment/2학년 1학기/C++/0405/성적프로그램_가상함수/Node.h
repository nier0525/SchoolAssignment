#pragma once

#include "Global.h"

template <typename c>
class Node {
private:
	c data;
	Node<c>* link;
public:
	Node<c>() {
		memset(&data, 0, sizeof(data));
		link = nullptr;
	}

	void SetData(c _data) {
		data = _data;
	}
	void SetLink(Node<c>* _link) {
		link = _link;
	}

	c GetData() {
		return data;
	}
	Node<c>* GetLink() {
		return link;
	}
};