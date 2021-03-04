#ifndef LISTNODE_H
#define LISTNODE_H

#include "Node.h"

template <class C>
class ListNode {
private:
	Node<C>*		Head;
public:
	ListNode() { Head = nullptr; }

	void Add(C*);
	void Del(C);
	C* Data();
};

template <class C>
void ListNode<C>::Add(C* _data) {
	Node<C>* newNode = (Node<C>*)malloc(sizeof(Node<C>));
	newNode->SetData(*_data);
	newNode->Setlink(nullptr);

	Node<C>* temp = Head;

	if (temp != nullptr) {
		while (temp->Link() != nullptr) {
			temp = temp->Link();
		}
		temp->Setlink(newNode);
	}
	else {
		Head = newNode;
		return;
	}
}

template <class C>
void ListNode<C>::Del(C _data) {
	Node<C>* temp = Head;
	if (temp == nullptr) { return; }
	if (temp->Link() == nullptr) {
		delete temp;
		Head = nullptr;
	}
	else {
		Node<C>*	pre;
		do {
			if (temp->GetData() == _data) { break; }
			pre = temp;
			temp = temp->Link();
		} while (temp != nullptr);
	}
	delete temp;
}

template <class C>
C* ListNode<C>::Data() {	
	return &Head->GetData();
}

#endif