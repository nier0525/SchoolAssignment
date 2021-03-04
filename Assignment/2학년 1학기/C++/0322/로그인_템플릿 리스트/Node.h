#pragma once

#include "Global.h"

template <typename c>
class Node {
private:
	c data;	// 노드의 데이터
	Node<c>* link; // 노드의 링크
public:
	Node(){
		memset(&data, 0, sizeof(data));
		link = nullptr;
	}
	~Node(){}

	// 읽기 함수
	Node<c>* Getlink() { return link; }
	c GetData() { return data; }

	// 쓰기 함수
	void Setlink(Node<c>* _link) { link = _link; }
	void Setdata(c _data) { data = _data; }
};