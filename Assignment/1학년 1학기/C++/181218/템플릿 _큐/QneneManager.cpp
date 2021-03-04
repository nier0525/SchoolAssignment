#include "QueueManager.h"

cQManager::cQManager() {}
cQManager::~cQManager() {}

void cQManager::intQ() {
	cQueue<int> myQueue;
	int n;
	cout << "ť�� ������ 5���� ���� : ";
	for (int i = 0; i < 5; i++) {
		cin >> n;
		myQueue.enqueue(n);
	}
	cout << "ť�� ���Ҹ� ������� �����ϸ� ��� : ";
	while (myQueue.length() != 0) {
		cout << myQueue.dequeue() << ' ';
	}
	cout << endl;
}

void cQManager::doubleQ() {
	cQueue<double> myQueue;
	double n;
	cout << "ť�� ������ 5���� �Ǽ� : ";
	for (int i = 0; i < 5; i++) {
		cin >> n;
		myQueue.enqueue(n);
	}
	cout << "ť�� ���Ҹ� ������� �����ϸ� ��� : ";
	while (myQueue.length() != 0) {
		cout << myQueue.dequeue() << ' ';
	}
	cout << endl;
}

void cQManager::charQ() {
	cQueue<char>* myQueue = new cQueue<char>;
	char n;
	cout << "ť�� ������ 5���� ���ڿ� : ";
	for (int i = 0; i < 5; i++) {
		cin >> n;
		myQueue->enqueue(n);
	}
	cout << "ť�� ���Ҹ� ������� �����ϸ� ��� : ";
	while (myQueue->length() != 0) {
		cout << myQueue->dequeue() << ' ';
	}
	cout << endl;
	delete myQueue;
}