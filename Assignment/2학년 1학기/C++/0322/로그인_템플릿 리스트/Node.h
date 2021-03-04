#pragma once

#include "Global.h"

template <typename c>
class Node {
private:
	c data;	// ����� ������
	Node<c>* link; // ����� ��ũ
public:
	Node(){
		memset(&data, 0, sizeof(data));
		link = nullptr;
	}
	~Node(){}

	// �б� �Լ�
	Node<c>* Getlink() { return link; }
	c GetData() { return data; }

	// ���� �Լ�
	void Setlink(Node<c>* _link) { link = _link; }
	void Setdata(c _data) { data = _data; }
};