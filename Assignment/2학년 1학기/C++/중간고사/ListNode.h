#pragma once

#include "Global.h"

template<typename c>		// ���ø�
class Node {
private:
	c data;		// ��� ������
	Node<c>* link;	// ��� �ּ�
public:
	Node<c>() { link = nullptr; }	// ��� �ʱ�ȭ
	~Node<c>() {}

	// ���� ������
	void SetData(c _data) { data = _data; }
	void SetLink(Node<c>* _link) { link = _link; }

	// �б� ������
	c GetData() { return data; }
	Node<c>* GetNext() { return link; }
};


template<typename c>
class ListNode {
private:
	Node<c>* head;	// ù ��带 �˸��� ��� ���
	int NodeCount;	// ��� ����
public:
	ListNode<c>();
	~ListNode<c>();

	void Init();	// ��� �ʱ�ȭ �Լ�
	void Input(c);	// ��� ����
	void Delete(Node<c>*);	// ��� ����

	bool isEmpty();	// ��尡 ����� �� Ȯ��
	bool isFull();	// ��尡 �� á�� �� Ȯ��

	Node<c>* GetHead() { return head; } // ��� ��� �б� ������
};

template<typename c>
ListNode<c>::ListNode() {
	head = nullptr;		// ��� ��� �ʱ�ȭ
	NodeCount = NODATA; // ��� ���� �ʱ�ȭ
}

template<typename c>
ListNode<c>::~ListNode() {
	Node<c>* ptr = head->GetNext();	// ��� ����� ���� ��尪 ����
	delete head;	// ��� ��� ����

	while (ptr != nullptr) { // ������ ��尡 ����(nullptr) ���������� �ݺ�
		head = ptr;	// ����带 ������ ���� �̵�
		ptr = ptr->GetNext(); // ������ ���� �ճ��� �̵�
		delete head; // ����� ��ȯ
	}
}

template<typename c>
void ListNode<c>::Init() {
	head = new Node<c>();	// ��� ���� �Ҵ�
	head = nullptr;			// �ʱ�ȭ
}

template<typename c>
bool ListNode<c>::isEmpty() {	// ��尡 ����� �� üũ
	if (NodeCount == NODATA) {
		return true;
	}
	else {
		return false;
	}
}

template<typename c>
bool ListNode<c>::isFull() { // ��尡 ���� á���� üũ
	if (NodeCount == LISTSIZE) {
		return true;
	}
	else {
		return false;
	}
}

template<typename c>
void ListNode<c>::Input(c _data) {
	if (isEmpty()) { // ��尡 ����� ��
		Init(); // ��� ��� ����
	}

	Node<c>* newNode = new Node<c>(); // ��� ���� �Ҵ�
	Node<c>* temp; // �� ��带 ���� ��� ������
	NodeCount++; // ��� ���� ���
	newNode->SetData(_data); // ��� �����Ϳ� ���ø� �Ű����� ����

	if (head == nullptr) { // ��尡 �ϳ��� ���
		head = newNode; // ��� ��� �տ� ��ġ
		return; // �Լ� ����
	}

	temp = head; // ������ ��尡 ��� ��忡 ��ġ��
	while (temp->GetNext() != nullptr) { // ������ ��尡 nullptr �� ����������
		temp = temp->GetNext(); // ������ ��� ������ �̵�
	}
	temp->SetLink(newNode); // ������ ��ġ�� ������ ��� ����
}

template<typename c>
void ListNode<c>::Delete(Node<c>* _node) {
	Node<c>* ptr;	// ������ ���

	if (head->GetNext() == nullptr) {	// ���� �� ��尡 �ϳ��� ���
		delete head; // ��� ��� ����
		head = nullptr; // ��� ��� �ʱ�ȭ
		NodeCount--; // ��� ���� ���� 
		return; // �Լ� ����
	}

	if (head == _node) { // ���� �� ��尡 ù��° �ڸ��� ���� ���
		ptr = head; // ������ ���� ��� ��忡 ��ġ
		head = head->GetNext(); // ��� ��� �� ���� �̵�
		delete ptr; // ������ ��尡 ��ġ�� ���� ��� ����(ù��° ���)
		NodeCount--; // ��� ���� ����
		return; // �Լ� ����
	}

	else {
		ptr = head; // ������ ���� �� �տ� ��ġ
		while (ptr->GetNext() != _node) { // ������ ��尡 �ŰԺ����� ã�� ������
			ptr = ptr->GetNext(); // ������ ��� �̵�
		}
		ptr->SetLink(_node->GetNext()); // ������ ��忡 ��ġ�� ����� �ּҸ� �Ű� ������
										// ��� �� ��忡 ����
		delete _node;					// �Ű� ���� ��� ����
		NodeCount--;					// ��� ���� ����
	}
}