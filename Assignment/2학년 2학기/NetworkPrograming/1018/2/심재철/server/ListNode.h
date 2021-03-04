#pragma once

#pragma comment (lib, "ws2_32")
#include <WinSock2.h>

#include <stdio.h>
#include <stdlib.h>

#define NODATA 0
#define MAX 100

template <typename T>
struct Node {
	T data;
	Node<T>* link;

	Node<T>() {
		ZeroMemory(&data, sizeof(data));
		link = nullptr;
	}

	Node<T>(T _data) {
		data = _data;
		link = nullptr;
	}
};

template <typename T>
struct List {
	Node<T>* head;
	Node<T>* tail;

	int count = NODATA;
};

template <typename T>
List<T>* init();

template <typename T>
void FreeList(List<T>*);

template <typename T>
bool Insert(List<T>*, T);

template <typename T>
bool Search(List<T>*, T);

template <typename T>
bool Delete(List<T>*, T);

template <typename T>
Node<T>* GetHead(List<T>* list) {
	return list->head;
}

template <typename T>
List<T>* init() {
	List<T>* list = new List<T>();

	list->head = new Node<T>();
	list->tail = list->head;

	return list;
}

template <typename T>
void FreeList(List<T>* list) {
	Node<T>* ptr = list->head->link;
	delete list->head;

	while (ptr != nullptr) {
		list->head = ptr;
		ptr = ptr->link;
		delete list->head;
	}
}

template <typename T>
bool Insert(List<T>* list, T data) {
	if (list->count > MAX) {
		printf("\n\n Node Full !\n\n");
		return false;
	}

	Node<T>* ptr = new Node<T>(data);

	list->tail->link = ptr;
	list->tail = ptr;

	list->count++;
	return true;
}
 
template <typename T>
bool Search(List<T>* list, T data) {
	if (list->count <= NODATA) {
		return false;
	}
	
	Node<T>* ptr = list->head;

	while (ptr != nullptr) {
		if (ptr->data == data) {
			return true;
		}
		ptr = ptr->link;
	}

	return false;
}

template <typename T>
bool Delete(List<T>* list, T data) {
	Node<T>* pre = list->head;
	Node<T>* ptr = pre->link;

	bool flug = false;

	while (ptr != nullptr) {
		if (ptr->data == data) {
			flug = true;
			pre->link = ptr->link;
			if (ptr->link == nullptr) {
				list->tail = pre;
			}
			delete ptr;
			list->count--;
			break;
		}
		pre = ptr;
		ptr = ptr->link;
	}

	if (flug) {
		return true;
	}
	return false;
}
