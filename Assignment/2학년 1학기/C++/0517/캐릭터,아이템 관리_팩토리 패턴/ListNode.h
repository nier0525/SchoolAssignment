#pragma once

#include "Global.h"

template <typename T>
class Node {
private:
	T data;
	Node<T>* next;
public:
	Node<T>() {
		memset(&data, 0, sizeof(data));
		next = nullptr;
	}
	Node<T>(T _data) {
		data = _data;
		next = nullptr;
	}

	~Node<T>() {

	}

	void SetData(T _data) {
		data = _data;
	}
	void SetNext(Node<T>* _next) {
		next = _next;
	}

	T GetData() { return data; }
	Node<T>* GetNext() { return next; }
};

template <typename T>
class ListNode {
private:
	Node<T>* head;
	Node<T>* Tail;
	int nCount;
public:
	ListNode<T>();
	~ListNode<T>();

	bool Insert(T _data);
	bool Delete(T _data);
	bool Remove(T _data);

	Node<T>* GetHead() { return head; }
	Node<T>* GetTail() { return Tail; }
	int GetNodeCount() { return nCount; }

	bool isEmpty();
	bool isFull();
};

template <typename T>
ListNode<T>::ListNode() {
	head = new Node<T>();
	Tail = head;
	nCount = NODATA;
}

template <typename T>
ListNode<T>::~ListNode() {
	Node<T>* ptr = head->GetNext();
	delete head;

	while (ptr != nullptr) {
		head = ptr;
		ptr = ptr->GetNext();
		delete head;
	}
}

template <typename T>
bool ListNode<T>::isEmpty() {
	if (nCount == NODATA) {
		return true;
	}
	return false;
}

template <typename T>
bool ListNode<T>::isFull() {
	if (nCount == LISTFULL) {
		return true;
	}
	return false;	
}

template <typename T>
bool ListNode<T>::Insert(T _data) {
	Node<T>* ptr = new Node<T>(_data);
	nCount++;
	Tail->SetNext(ptr);
	Tail = ptr;
	return true;
}

template <typename T>
bool ListNode<T>::Delete(T _data) {
	Node<T>* pre = head;
	Node<T>* ptr = pre->GetNext();
	bool flug = false;

	while (ptr != nullptr) {
		if (ptr->GetData() == _data) {
			flug = true;
			pre->SetNext(ptr->GetNext());
			if (ptr->GetNext() == nullptr) {
				Tail = pre;
			}
			delete ptr;
			nCount--;
			break;
		}
		pre = ptr;
		ptr = ptr->GetNext();
	}

	if (flug) {
		return true;
	}
	return false;
}

template <typename T>
bool ListNode<T>::Remove(T _data) {
	Node<T>* pre = head;
	Node<T>* ptr = pre->GetNext();
	bool flug = false;

	while (ptr != nullptr) {
		if (ptr->GetData() == _data) {
			flug = true;
			pre->SetNext(ptr->GetNext());
			if (ptr->GetNext() == nullptr) {
				Tail = pre;
			}
			delete ptr->GetData();
			delete ptr;
			nCount--;
			break;
		}
		pre = ptr;
		ptr = ptr->GetNext();
	}

	if (flug) {
		return true;
	}
	return false;
}