#ifndef NODE_H
#define NODE_H

#include "Base.h"

template <typename T>
struct Node {
	T data;
	Node* link;
};

template <typename T>
struct LinkedList {
	Node<T>* Head;
};

template<typename T, int SIZE>
class ListNode {
private:
	int count;
public:
	ListNode();
	~ListNode();
	
	LinkedList<T>* CreateNode(void);
	void FirstAdd(LinkedList<T>*, T);
	void MiddleAdd(LinkedList<T>*, Node<T>*, T);
	void LastAdd(LinkedList<T>*,T);

	void Delete(LinkedList<T>*,Node<T>*);
	Node<T>* Search(LinkedList<T>*,T);

	void Display(LinkedList<T>*);
	void Reset(LinkedList<T>*);

	bool isEmpty();
	bool isFull();
};

template <typename T, int SIZE>
LinkedList<T>* ListNode<T, SIZE>::CreateNode(void) {
	LinkedList<T>* L;
	L = (LinkedList<T>*)malloc(sizeof(LinkedList<T>));
	L->Head = nullptr;
	return L;
}

template <typename T, int SIZE>
ListNode<T,SIZE>::ListNode() {
	count = 0;
}

template <typename T, int SIZE>
ListNode<T,SIZE>::~ListNode() {}

template <typename T, int SIZE>
bool ListNode<T, SIZE>::isEmpty() {
	if (count == 0) {
		return true;
	}
	else {
		return false;
	}
}

template <typename T, int SIZE>
bool ListNode<T, SIZE>::isFull() {
	if (count > SIZE) {
		return true;
	}
	else {
		return false;
	}
}

template <typename T, int SIZE>
void ListNode<T, SIZE>::FirstAdd(LinkedList<T>* L, T _data) {
	Node<T>* newNode;
	count++;
	newNode = (Node<T>*)malloc(sizeof(Node<T>));
	newNode->data = _data;
	newNode->link = L->Head;
	L->Head = newNode;
}

template <typename T, int SIZE>
void ListNode<T, SIZE>::MiddleAdd(LinkedList<T>* L, Node<T>* p, T _data) {
	Node<T>* newNode;
	count++;
	newNode = (Node<T>*)malloc(sizeof(Node<T>));
	newNode->data = _data;
	if (L->Head == nullptr) {
		newNode->link = nullptr;
		L->Head = newNode;
	}
	newNode->link = p->link;
	p->link = newNode;
}

template <typename T, int SIZE>
void ListNode<T, SIZE>::LastAdd(LinkedList<T>* L, T _data) {
	
	if (isFull()) {
		cout << "Node Full" << endl;
		return;
	}
	else {
		Node<T>*	newNode;
		Node<T>*	Temp;

		newNode = (Node<T>*)malloc(sizeof(Node<T>));
		newNode->data = _data;
		newNode->link = nullptr;
		count++;

		if (L->Head == nullptr) { L->Head = newNode; return; }
		else {
			Temp = L->Head;

			while (Temp->link != nullptr) {
				Temp = Temp->link;
			}
			Temp->link = newNode;
		}
	}
}

template <typename T, int SIZE> 
void ListNode<T,SIZE>::Delete(LinkedList<T>* L, Node<T>* p)
{
	if (isEmpty()) {
		cout << "Node Empty" << endl;
		return;
	}
	Node<T>* pre;

	if (L->Head->link == nullptr) {
		delete (L->Head);
		L->Head = nullptr;
		return;
	}
	else if (p == nullptr) {
		cout << "삭제 할 노드를 찾지 못했습니다." << endl;
		return; 
	}
	else {
		pre = L->Head;
		while (pre->link != p) {
			pre = pre->link;
		}
		pre->link = p->link;
		delete p;
	}
	count--;
}

template <typename T, int SIZE>
Node<T>* ListNode<T, SIZE>::Search(LinkedList<T>* L, T _data) {
	Node<T>* temp;
	temp = L->Head;
	while (temp != nullptr) {
		if (temp->data == _data) {
			return temp;
		}
		else {
			temp = temp->link;
		}
	}
	return temp;
}

template <typename T, int SIZE>
void ListNode<T, SIZE>::Display(LinkedList<T>* L) {
	Node<T>* p;
	
	cout << "[ ";
	p = L->Head;
	while (p != nullptr) 
	{
		cout << p->data;
		p = p->link;
		if (p != nullptr) cout << " , ";
	}
	cout << " ]" << endl;
}

template <typename T, int SIZE>
void ListNode<T, SIZE>::Reset(LinkedList<T>* L) {
	Node<T>* p;
	while (L->Head != nullptr) {
		p = L->Head;
		L->Head = L->Head->link;
		delete p;
		p = nullptr;
	}
	count = 0;
}

#endif