#pragma once

#include "Node.h"

template <typename c>
class ListNode {
private:
	Node<c>* head; // 첫번째 노드를 가르킬 더미 노드
	int count; // 노드 개수
public:
	ListNode() { head = nullptr; count = 0; } // 멤버 변수 초기화
	~ListNode(); // 할당 받은 메모리 반환
	// 첫 리스트를 만들 때 초기화 함수
	void init() { head = new Node<c>; head = nullptr; } 
	// 삽입,삭제 함수
	void Insert(c);
	void Delete(Node<c>*);

	//Node<c>* Serach(c);
	
	bool isEmpty();
	bool isFull();

	// 읽기 함수
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

// 노드가 비었을 때
template <typename c>
bool ListNode<c>::isEmpty() {
	if (count == NODATA) {
		return true;
	}
	else {
		return false;
	}
}

// 노드가 꽉 찼을 때
template <typename c>
bool ListNode<c>::isFull() {
	if (count == USERSIZE) {
		return true;
	}
	else {
		return false;
	}
}

// 삽입
template <typename c>
void ListNode<c>::Insert(c _data) {
	if (isFull()) {
		cout << "만들 수 있는 계정의 개수가 최대입니다." << endl;
		return;
	}
	// 삽입되는 노드는 항상 맨 앞 노드로 연결댐
	Node<c>* newNode; 
	count++; 
	newNode = new Node<c>(); 
	newNode->Setdata(_data); 
	newNode->Setlink(head);
	head = newNode;
}

// 삭제
template <typename c>
void ListNode<c>::Delete(Node<c>* _node) {
	Node<c>* pre;

	if (head->Getlink() == nullptr) { // 노드가 하나일 때
		delete head;
		head = nullptr;
		count--;
		return;
	}
	if (head == _node) { // 첫번째 노드 처리
		pre = head;
		head = head->Getlink();
		delete pre;
		count--;
		return;
	}
	else {				// 중간, 마지막 노드 처리
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
