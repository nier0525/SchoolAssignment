#pragma once

#include "Student.h"
#include "ListNode.h"

class DBManager {
protected:
	ListNode<CStudent*>* list;

	Node<CStudent*>* NowStudent;
public:
	DBManager();
	~DBManager();

	virtual void input() = 0;
	virtual void display() = 0;
	virtual void search() = 0;

};