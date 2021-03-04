#pragma once

#include "Global.h"

// ���ø� ��ũ�� ����Ʈ

// ��� Ŭ����

template <typename T>
class Node {
private:
	T data;	// �������� �ʴ� �ڷ����� ����� �������̴�.
	Node<T>* next;	// ����� ��ũ
public:
	// ���� �ʱ�ȭ
	Node<T>() {
		memset(&data, 0, sizeof(data));
		next = nullptr;
	}
	// ������ �����ε�
	Node<T>(T _data) {
		data = _data;
		next = nullptr;
	}

	~Node<T>() {

	}

	// ���� ������
	void SetData(T _data) {
		data = _data;
	}
	void SetNext(Node<T>* _next) {
		next = _next;
	}

	// �б� ������
	T GetData() { return data; }
	Node<T>* GetNext() { return next; }
};

// ��ũ�� ����Ʈ

template <typename T>
class ListNode {
private:
	Node<T>* head;	// ù ���
	Node<T>* Tail;	// ������ ���
public:
	ListNode<T>();
	~ListNode<T>();

	bool Insert(T _data);	 // ����
	bool Delete(T _data);	// ���� (��常)
	bool Remove(T _data);	// ���� (�����ͱ���)

	// ���� ����¿� Ȥ���� ���ɱ� �;� ������ �޼ҵ�
	//void SaveList(FILE* fp, Node<T>* p);
	//Node<T>* LoadList(FILE* fp);

	// �б� ������
	Node<T>* GetHead() { return head; }
	Node<T>* GetTail() { return Tail; }
};


// �ʱ�ȭ
template <typename T>
ListNode<T>::ListNode() {
	head = new Node<T>();	// ù ��� ���� �Ҵ�
	Tail = head;			// ������ ��� ��ġ �ʱ�ȭ
}

template <typename T>
ListNode<T>::~ListNode() {
	Node<T>* ptr = head->GetNext();	// ù ����� ���� ��� ��ġ�� ����
	delete head;	// ù ��� ����

	while (ptr != nullptr) {	// ���� ��尡 �� �� �̻��̶��
		head = ptr;	// ��� ��忡 ���� ��� �������� ��ġ ����
		ptr = ptr->GetNext(); // ��� �����ʹ� ���� ����
		delete head;	// ��� ��� ��ġ�� ��� ����
	} // �ᱹ ������ ��尡 nullptr �� ����������(������ ���) ��ȸ�ϸ鼭 ��� ��带 �����Ѵ�.
}

// ����
template <typename T>
bool ListNode<T>::Insert(T _data) {
	Node<T>* ptr = new Node<T>(_data); // ��� ��ü ���� �� �Ű� ������ ���� �����͸� ��� �����Ϳ� ����
	Tail->SetNext(ptr);	// ������ ����� ��ũ�� ���� ������ ���� ����
	Tail = ptr;	// ���� ������ ���� ������ ��带 ����
	return true;
}


// ���� (��常)
template <typename T>
bool ListNode<T>::Delete(T _data) {
	Node<T>* pre = head; // �ӽ� ��忡 ù ��� ����
	Node<T>* ptr = pre->GetNext();	// �ӽ� ���2 �� ù ����� ���� ����
	bool flug = false;

	while (ptr != nullptr) { // �ӽ� ���2 �� �����͸� ã�� ���ϸ� ���������� ��ȸ��
		if (ptr->GetData() == _data) {	// ��ġ�ϴ� �����͸� ã�Ҵٸ�
			flug = true;
			pre->SetNext(ptr->GetNext());	 // �ӽ� ����� ��ũ�� �ӽ� ���2 �� ���� ��忡 ����
			if (ptr->GetNext() == nullptr) {	 // ���� �ӽ� ���2 �� ���� ��尡 �������̶��
				Tail = pre;	// ������ ��忡 �ӽ� ���2�� ����
			}
			delete ptr;	// �ӽ� ��� 2 ��ġ�� ��� ����
			break; // ��ȸ �ߴ�
		}
		pre = ptr;	// �ӽ� ��� �� ĭ ������ �̵�
		ptr = ptr->GetNext();	// �ӽ� ���2 �̵�
	}

	if (flug) {	// �����͸� �����ߴٸ�
		return true;
	}
	return false;
}

// ���� (�����ͱ���)
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
			delete ptr->GetData();	// Delete �� ���������� �� �κп� ��� ���� �����ͱ��� �����Ѵٴ� �������� �ִ�.
			delete ptr;
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

// �Ʒ��� ���� ����� �� Ȥ�ó� ���� �;� ���� �޼ҵ�

//template <typename T>
//void ListNode<T>::SaveList(FILE* fp, Node<T>* p) {
//	while (p != nullptr) {
//		fwrite(p, sizeof(p), 1, fp);
//		p = p->GetNext();
//	}
//}
//
//template <typename T>
//Node<T>* ListNode<T>::LoadList(FILE* fp) {
//	Node<T>* p = nullptr;
//
//	while (1) {
//		Node<T>* ptr = new Node<T>*();
//		int isRead = fread(ptr, sizeof(ptr), 1, fp);
//
//		if (isRead) {
//			break;
//		}
//
//		if (head == nullptr) {
//			head = ptr;
//			p = ptr;
//		}
//		else {
//			p->GetNext() = ptr;
//			p = p->GetNext();
//		}
//	}
//
//	p->GetNext() = nullptr;
//	return head;
//}