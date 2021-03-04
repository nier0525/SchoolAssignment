#include "Queue.h"

template <class T>
cQueue<T>::cQueue() {
	top = EMPTY;
	bottom = EMPTY;
}

template <class T>
cQueue<T>::~cQueue() {}

template <class T>
void cQueue<T>::enqueue(T element) {
	if (top >= QSIZE) {
		cout << "Queue Full";
		return;
	}
	data[++top] = element;
}

template <class T>
T cQueue<T>::dequeue() {
	if (top == EMPTY) {
		cout << "Queue Empty ";
		return false;
	}
	T retData;
	retData = data[++bottom];
	return retData;
}

template <class T>
int cQueue<T>::length() {
	return top - bottom;
}
