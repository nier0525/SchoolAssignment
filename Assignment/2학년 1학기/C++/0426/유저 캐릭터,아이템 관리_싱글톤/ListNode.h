#pragma once

#include "Global.h"

template <typename c>
class Node {
private:
	c data;
	Node<c>* link;
public:
	Node() { 
		memset(&data, 0, sizeof(data));
		link = nullptr;
	}
	Node(c _data) {
		data = _data;
		link = nullptr;
	}
	~Node() { }

	void SetData(c _data) { data = _data; }
	void SetLink(Node<c>* _link) { link = _link; }

	c GetData() { return data; }
	Node<c>* GetLink() { return link; }
};

template <typename c>
class ListNode {
private:
	Node<c>* head;
	Node<c>* tail;
	int NodeCount;
public:
	ListNode();
	~ListNode();

	bool isEmpty();
	bool isFull();

	bool Insert(c);
	bool Delete(c);
	bool Remove(c);

	Node<c>* GetHead() { return head; }
	Node<c>* GetTail() { return tail; }
	int GetNodeCount() { return NodeCount; }
};

template <typename c>
ListNode<c>::ListNode() {
	head = new Node<c>();
	tail = head;
	NodeCount = NODATA;
}

template <typename c>
ListNode<c>::~ListNode() {
	Node<c>* ptr = head->GetLink();
	delete head;

	while (ptr != nullptr) {
		head = ptr;
		ptr = ptr->GetLink();
		delete head;
	}
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
	if (NodeCount == LISTSIZE) {
		return true;
	}
	else {
		return false;
	}
}

template <typename c>
bool ListNode<c>::Insert(c _data) {
	// if (isFull()) { return false; }
	Node<c>* ptr = new Node<c>(_data);
	NodeCount++;
	tail->SetLink(ptr);
	tail = ptr;
	return true;
}

template <typename c>
bool ListNode<c>::Delete(c _data) {

	Node<c>* pre = head;
	Node<c>* ptr = pre->GetLink();
	bool flug = false;

	while (ptr != nullptr) {
		if (ptr->GetData() == _data) {
			flug = true;
			pre->SetLink(ptr->GetLink());
			if (ptr->GetLink() == nullptr) {
				tail = pre;
			}
			delete ptr;
			NodeCount--;
			break;
		}
		pre = ptr;
		ptr = ptr->GetLink();
	}

	if (flug) {
		return true;
	}
	return false;
}

template <typename c>
bool ListNode<c>::Remove(c _data) {

	Node<c>* pre = head;
	Node<c>* ptr = pre->GetLink();
	bool flug = false;

	while (ptr != nullptr) {
		if (ptr->GetData() == _data) {
			flug = true;
			pre->SetLink(ptr->GetLink());
			if (ptr->GetLink() == nullptr) {
				tail = pre;
			}
			delete ptr->GetData();
			delete ptr;
			NodeCount--;
			break;
		}
		pre = ptr;
		ptr = ptr->GetLink();
	}

	if (flug) {
		return true;
	}
	return false;
}