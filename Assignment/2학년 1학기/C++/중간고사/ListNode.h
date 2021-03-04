#pragma once

#include "Global.h"

template<typename c>		// 템플릿
class Node {
private:
	c data;		// 노드 데이터
	Node<c>* link;	// 노드 주소
public:
	Node<c>() { link = nullptr; }	// 노드 초기화
	~Node<c>() {}

	// 쓰기 접근자
	void SetData(c _data) { data = _data; }
	void SetLink(Node<c>* _link) { link = _link; }

	// 읽기 접근자
	c GetData() { return data; }
	Node<c>* GetNext() { return link; }
};


template<typename c>
class ListNode {
private:
	Node<c>* head;	// 첫 노드를 알리는 헤드 노드
	int NodeCount;	// 노드 개수
public:
	ListNode<c>();
	~ListNode<c>();

	void Init();	// 노드 초기화 함수
	void Input(c);	// 노드 생성
	void Delete(Node<c>*);	// 노드 삭제

	bool isEmpty();	// 노드가 비었는 지 확인
	bool isFull();	// 노드가 꽉 찼는 지 확인

	Node<c>* GetHead() { return head; } // 헤드 노드 읽기 접근자
};

template<typename c>
ListNode<c>::ListNode() {
	head = nullptr;		// 헤드 노드 초기화
	NodeCount = NODATA; // 노드 개수 초기화
}

template<typename c>
ListNode<c>::~ListNode() {
	Node<c>* ptr = head->GetNext();	// 헤드 노드의 다음 노드값 대입
	delete head;	// 헤드 노드 삭제

	while (ptr != nullptr) { // 포인터 노드가 끝을(nullptr) 만날때까지 반복
		head = ptr;	// 헤드노드를 포인터 노드로 이동
		ptr = ptr->GetNext(); // 포인터 노드는 앞노드로 이동
		delete head; // 헤드노드 반환
	}
}

template<typename c>
void ListNode<c>::Init() {
	head = new Node<c>();	// 노드 동적 할당
	head = nullptr;			// 초기화
}

template<typename c>
bool ListNode<c>::isEmpty() {	// 노드가 비었는 지 체크
	if (NodeCount == NODATA) {
		return true;
	}
	else {
		return false;
	}
}

template<typename c>
bool ListNode<c>::isFull() { // 노드가 가득 찼는지 체크
	if (NodeCount == LISTSIZE) {
		return true;
	}
	else {
		return false;
	}
}

template<typename c>
void ListNode<c>::Input(c _data) {
	if (isEmpty()) { // 노드가 비었을 때
		Init(); // 헤드 노드 생성
	}

	Node<c>* newNode = new Node<c>(); // 노드 동적 할당
	Node<c>* temp; // 끝 노드를 위한 노드 포인터
	NodeCount++; // 노드 개수 상승
	newNode->SetData(_data); // 노드 데이터에 템플릿 매개변수 대입

	if (head == nullptr) { // 노드가 하나일 경우
		head = newNode; // 헤드 노드 앞에 위치
		return; // 함수 종료
	}

	temp = head; // 포인터 노드가 헤드 노드에 위치함
	while (temp->GetNext() != nullptr) { // 포인터 노드가 nullptr 를 만날때까지
		temp = temp->GetNext(); // 포인터 노드 앞으로 이동
	}
	temp->SetLink(newNode); // 마지막 위치에 생성한 노드 연결
}

template<typename c>
void ListNode<c>::Delete(Node<c>* _node) {
	Node<c>* ptr;	// 포인터 노드

	if (head->GetNext() == nullptr) {	// 삭제 할 노드가 하나인 경우
		delete head; // 헤드 노드 삭제
		head = nullptr; // 헤드 노드 초기화
		NodeCount--; // 노드 개수 감소 
		return; // 함수 종료
	}

	if (head == _node) { // 삭제 할 노드가 첫번째 자리에 있을 경우
		ptr = head; // 포인터 노드는 헤드 노드에 위치
		head = head->GetNext(); // 헤드 노드 앞 노드로 이동
		delete ptr; // 포인터 노드가 위치한 곳의 노드 삭제(첫번째 노드)
		NodeCount--; // 노드 개수 감소
		return; // 함수 종료
	}

	else {
		ptr = head; // 포인터 노드는 맨 앞에 위치
		while (ptr->GetNext() != _node) { // 포인터 노드가 매게변수를 찾을 때까지
			ptr = ptr->GetNext(); // 포인터 노드 이동
		}
		ptr->SetLink(_node->GetNext()); // 포인터 노드에 위치한 노드의 주소를 매게 변수의
										// 노드 뒤 노드에 연결
		delete _node;					// 매게 변수 노드 삭제
		NodeCount--;					// 노드 개수 감소
	}
}