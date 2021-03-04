#pragma once

#include "global.h"

template <typename T>
struct Node {
	T data;
	Node<T>* L_link;
	Node<T>* R_link;
};

template <typename T>
struct ListNode {
	Node<T>* head;
	Node<T>* tail;
};

template <typename T>
void DL_CreateListNode(ListNode<T>** list) {
	(*list) = (ListNode<T>*)malloc(sizeof(ListNode<T>));

	(*list)->head = (Node<T>*)malloc(sizeof(Node<T>));
	(*list)->tail = (Node<T>*)malloc(sizeof(Node<T>));

	(*list)->head->R_link = (*list)->tail;
	(*list)->head->L_link = (*list)->head;
	(*list)->tail->R_link = (*list)->tail;
	(*list)->tail->L_link = (*list)->head;
}

template <typename T>
void DL_FreeListNode(ListNode<T>* list) {
	Node<T>* ptr = list->head->R_link;
	free(list->head);

	while (ptr != list->tail) {
		list->head = ptr;
		ptr = ptr->R_link;
		free(list->head);
	}

	free(list);
}

template <typename T>
int DL_Insert(ListNode<T>* list, T data) {
	Node<T>* before = list->head;
	Node<T>* newNode = (Node<T>*)malloc(sizeof(Node<T>));
	newNode->data = data;

	newNode->R_link = before->R_link;
	newNode->L_link = before;
	before->R_link->L_link = newNode;
	before->R_link = newNode;

	return 0;
}

template <typename T>
int DL_Delete(ListNode<T>* list, Node<T>* data) {

	//Node<T>* temp = AL_Search(list, data);
	Node<T>* temp = data;
	if (temp == nullptr) {
		return -1;
	}

	Node<T>* before = temp->L_link;
	Node<T>* after = temp->R_link;

	before->R_link = after;
	after->L_link = before;

	delete temp;
	return 0;
}

template <typename T>
Node<T>* DL_Search(ListNode<T>* list, T data) {

	Node<T>* ptr = list->head->R_link;

	while (ptr != list->tail) {
		if (ptr->data == data) {
			return ptr;
		}
		ptr = ptr->R_link;
	}

	return nullptr;
}
