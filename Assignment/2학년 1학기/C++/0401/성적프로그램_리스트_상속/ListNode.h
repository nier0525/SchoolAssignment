#pragma once

#include "Node.h"

template <typename c>
class ListNode {
private:
	Node<c>* head;
	int mCount;
public:
	ListNode() { head = nullptr; mCount = 0; }
	~ListNode();

	void init() { head = new Node<c>; head = nullptr; }
	void insert(c);
	void Delete(Node<c>*);

	bool isEmpty();
	bool isFull();

	Node<c>* GetHead() { return head; }
	int GetCount() { return mCount; }
};

template <typename c>
ListNode<c>::~ListNode() {
	Node<c>* ptr = head->Getlink();
	delete head;

	while (ptr != nullptr) {
		head = ptr;
		ptr = ptr->Getlink();
		delete head;
	}
}

template <typename c>
bool ListNode<c>::isEmpty() {
	if (mCount == NODATA) {
		return true;
	}
	else {
		return false;
	}
}

template <typename c>
bool ListNode<c>::isFull() {
	if (mCount == LISTSIZE) {
		return true;
	}
	else {
		return false;
	}
}

template <typename c>
void ListNode<c>::insert(c _data) {
	if (isEmpty()) {
		init();
	}
	if (isFull()) {
		cout << "최대 인원 초과" << endl;
		return;
	}
	Node<c>* newNode = new Node<c>();
	Node<c>* temp;
	mCount++;
	newNode->Setdata(_data);
	if (head == nullptr) { 
		head = newNode;
		return; 
	}

	temp = head;
	while (temp->Getlink() != nullptr) {
		temp = temp->Getlink();
	}
	temp->Setlink(newNode);
}

template <typename c>
void ListNode<c>::Delete(Node<c>* _node) {
	Node<c>* pre;

	if (head->Getlink() == nullptr) {
		delete head;
		head = nullptr;
		mCount--;
		return;
	}

	if (head == _node) {
		pre = head;
		head = head->Getlink();
		delete pre;
		mCount--;
		return;
	}
	else {
		pre = head;
		while (pre->Getlink() != _node) {
			pre = pre->Getlink();
		}
		pre->Setlink(_node->Getlink());
		delete _node;
	}
	mCount--;
}
