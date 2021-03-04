//#include "Queue.cpp"
#include "Manager.h"
int main() {
	cManager* p = new cManager();
	p->run();
	delete p;
	return 0;
	/*
	cQueue<int*> iQueue;
	int *p = new int[5];

	for (int i = 0; i < 5; i++) {
		p[i] = i * 5;
	}

	iQueue.enqueue(p);

	int *q = iQueue.dequeue();
	for (int i = 0; i < 5; i++) {
		cout << q[i] << ' ';
	}
	cout << endl;
	delete[] p;

	cQueue<double> dQueue;
	double d[5] = { 0.5 , 1.5, 2.5, 3.5, 4.5 };
	for (int i = 0; i < 5; i++) {
		double b;
		b = d[i];
		dQueue.enqueue(b);
	}
	for (int i = 0; i < 5; i++) {
		cout << dQueue.dequeue() << ' ';
	}
	cout << endl;

	cQueue<char> *c = new cQueue<char>();
	c->enqueue('a');
	c->enqueue('b');
	for (int i = 0; i < 2; i++) {
		cout << c->dequeue() << ' ';
	}
	delete c;
	*/
}