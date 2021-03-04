#ifndef QUEUE_H
#define QUEUE_H
#include "Base.h"

template < class T >

class cQueue {
private:
	int top;
	int bottom;

	T data[QSIZE];
public:
	cQueue();
	~cQueue();
	void enqueue(T);
	int length();
	T dequeue();
};

#endif