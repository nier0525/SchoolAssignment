#pragma once

#include "Node.h"

template <typename c>
class ListNode {
private:
	Node<c>* head;
	int NodeCount;
public:
	ListNode<c>();
	~ListNode<c>();

	void init();
	void Input(c);
	void Delete(Node<c>*);

	bool isEmpty();
	bool isFull();

	Node<c>* Gethead() { return head; }
	int GetNodeCount() { return NodeCount; }
};

template <typename c>
ListNode<c>::ListNode() {
	head = nullptr;
	NodeCount = NODATA;
}

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
void ListNode<c>::init() {
	head = new Node<c>();
	head = nullptr;
}

template <typename c>
bool ListNode<c>::isEmpty() {
	if (NodeCount == NODATA) {
		return true;
	}
	else {
		return false;
	}
}

template <typename c>
bool ListNode<c>::isFull() {
	if (NodeCount == MAX) {
		return true;
	}
	else {
		return false;
	}
}

template <typename c>
void ListNode<c>::Input(c _data) {
	if (isEmpty()) {
		init();
	}
	if (isFull()) {
		cout << "Node is Full.. Please Delete Node" << endl;
		return;
	}

	Node<c>* newNode = new Node<c>();
	Node<c>* temp;
	NodeCount++;
	newNode->Setdata(_data);

	if (head == nullptr) {
		head = newNode;
		return;
	}

	temp = head;
	while (temp->Getlink() != nullptr) {
		temp = temp->Getlink();
	}
	temp->SetLink(newNode);
}

template <typename c>
void ListNode<c>::Delete(Node<c>* _node) {
	Node<c>* ptr;
	if (head->Getlink() == nullptr) {
		delete head;
		head = nullptr;
		NodeCount--;
		return;
	}

	if (head == _node) {
		ptr = head;
		head = head->Getlink();
		delete ptr;
		NodeCount--;
		return;
	}

	else {
		ptr = head;
		while (ptr->Getlink() != _node) {
			ptr = ptr->Getlink();
		}
		ptr->SetLink(_node->Getlink());
		delete _node;
		NodeCount--;
	}
}