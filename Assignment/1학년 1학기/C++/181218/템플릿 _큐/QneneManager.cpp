#include "QueueManager.h"

cQManager::cQManager() {}
cQManager::~cQManager() {}

void cQManager::intQ() {
	cQueue<int> myQueue;
	int n;
	cout << "큐에 삽입할 5개의 정수 : ";
	for (int i = 0; i < 5; i++) {
		cin >> n;
		myQueue.enqueue(n);
	}
	cout << "큐의 원소를 순서대로 제거하며 출력 : ";
	while (myQueue.length() != 0) {
		cout << myQueue.dequeue() << ' ';
	}
	cout << endl;
}

void cQManager::doubleQ() {
	cQueue<double> myQueue;
	double n;
	cout << "큐에 삽입할 5개의 실수 : ";
	for (int i = 0; i < 5; i++) {
		cin >> n;
		myQueue.enqueue(n);
	}
	cout << "큐의 원소를 순서대로 제거하며 출력 : ";
	while (myQueue.length() != 0) {
		cout << myQueue.dequeue() << ' ';
	}
	cout << endl;
}

void cQManager::charQ() {
	cQueue<char>* myQueue = new cQueue<char>;
	char n;
	cout << "큐에 삽입할 5개의 문자열 : ";
	for (int i = 0; i < 5; i++) {
		cin >> n;
		myQueue->enqueue(n);
	}
	cout << "큐의 원소를 순서대로 제거하며 출력 : ";
	while (myQueue->length() != 0) {
		cout << myQueue->dequeue() << ' ';
	}
	cout << endl;
	delete myQueue;
}