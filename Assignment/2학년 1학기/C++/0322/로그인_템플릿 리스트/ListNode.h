#pragma once

#include "Node.h"

template <typename c>
class ListNode {
private:
	Node<c>* head; // ù��° ��带 ����ų ���� ���
	int count; // ��� ����
public:
	ListNode() { head = nullptr; count = 0; } // ��� ���� �ʱ�ȭ
	~ListNode(); // �Ҵ� ���� �޸� ��ȯ
	// ù ����Ʈ�� ���� �� �ʱ�ȭ �Լ�
	void init() { head = new Node<c>; head = nullptr; } 
	// ����,���� �Լ�
	void Insert(c);
	void Delete(Node<c>*);

	//Node<c>* Serach(c);
	
	bool isEmpty();
	bool isFull();

	// �б� �Լ�
	Node<c>* Gethead() { return head; }
};

//--------------------------------------------------------------

template <typename c>
ListNode<c>::~ListNode() {
	//Node<c>* node = head->Getlink();

	delete head;
	//while (node != nullptr) {
	//	head = node;
	//	node = node->Getlink();
	//	delete head;
	//}
}

// ��尡 ����� ��
template <typename c>
bool ListNode<c>::isEmpty() {
	if (count == NODATA) {
		return true;
	}
	else {
		return false;
	}
}

// ��尡 �� á�� ��
template <typename c>
bool ListNode<c>::isFull() {
	if (count == USERSIZE) {
		return true;
	}
	else {
		return false;
	}
}

// ����
template <typename c>
void ListNode<c>::Insert(c _data) {
	if (isFull()) {
		cout << "���� �� �ִ� ������ ������ �ִ��Դϴ�." << endl;
		return;
	}
	// ���ԵǴ� ���� �׻� �� �� ���� �����
	Node<c>* newNode; 
	count++; 
	newNode = new Node<c>(); 
	newNode->Setdata(_data); 
	newNode->Setlink(head);
	head = newNode;
}

// ����
template <typename c>
void ListNode<c>::Delete(Node<c>* _node) {
	Node<c>* pre;

	if (head->Getlink() == nullptr) { // ��尡 �ϳ��� ��
		delete head;
		head = nullptr;
		count--;
		return;
	}
	if (head == _node) { // ù��° ��� ó��
		pre = head;
		head = head->Getlink();
		delete pre;
		count--;
		return;
	}
	else {				// �߰�, ������ ��� ó��
		pre = head;
		while (pre->Getlink() != _node) {
			pre = pre->Getlink();
		}
		pre->Setlink(_node->Getlink());
		delete _node;
	}
	count--;
}


//template <typename c>
//Node<c>* ListNode<c>::Serach(c _data) {
//	Node<c>* temp;
//	temp = head;
//	while (temp != nullptr) {
//		if (temp->GetData() == _data) {
//			return temp;
//		}
//		else {
//			temp = temp->Getlink();
//		}
//	}
//	return temp;
//}
