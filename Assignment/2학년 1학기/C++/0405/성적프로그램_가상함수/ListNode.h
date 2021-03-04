#pragma once

#include "Node.h"

template<typename c>
class ListNode {
private:
	Node<c>* head;
	int NodeCount;
public:
	ListNode<c>() {
		head = nullptr;
		NodeCount = 0;
	}
	~ListNode<c>();

	void init() { 
		head = new Node<c>(); 
		head = nullptr;
	}

	void Input(c);
	//void Delete(Node<c>*)

	bool isEmpty();
	bool isFull();

	int GetNodeCount() {
		return NodeCount;
	}
	Node<c>* GetHead() {
		return head;
	}
};

template<typename c>
ListNode<c>::~ListNode() {
	Node<c>* ptr = head->GetLink();
	delete head;

	while (ptr != nullptr) {
		head = ptr;
		ptr = ptr->GetLink();
		delete head;
	}
}

template<typename c>
bool ListNode<c>::isEmpty() {
	if (NodeCount == NODATA) {
		return true;
	}
	else {
		return false;
	}
}

template<typename c>
bool ListNode<c>::isFull() {
	if (NodeCount == MAXLIST) {
		return true;
	}
	else {
		return false;
	}
}

template<typename c>
void ListNode<c>::Input(c _data) {
	if (isEmpty()) {
		init();
	}
	if (isFull()) {
		cout << "인원 초과 !" << endl;
		return;
	}

	Node<c>* newNode = new Node<c>();
	Node<c>* temp;
	NodeCount++;

	newNode->SetData(_data);
	if (head == nullptr) {
		head = newNode;
		return;
	}

	temp = head;
	while (temp->GetLink() != nullptr) {
		temp = temp->GetLink();
	}
	temp->SetLink(newNode);
}

//template<typename c>
//void ListNode<c>::Delete(Node<c>* _node) {
//	Node<c>* pre;
//	if (head->GetLink() == nullptr) {
//		delete head;
//		head = nullptr;
//		NodeCount--;
//		return;
//	}
//
//	if (head == _node) {
//		pre = head;
//		head = head->GetLink();
//		delete pre;
//		NodeCount--;
//		return;
//	}
//
//	else {
//		pre = head;
//		while (pre->GetLink() != _node) {
//			pre = pre->GetLink();
//		}
//		pre->SetLink(_node->GetLink());
//		delete _node;
//	}
//	NodeCount--;
//}