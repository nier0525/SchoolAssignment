#pragma once

#include "Student.h"
#include "ListNode.h"

class CSchoolManager {
protected:
	ListNode<CStudent*>* StudentList;
	Node<CStudent*>* NowStudent;
public:
	CSchoolManager();
	virtual ~CSchoolManager();

	virtual void Input() = 0;
	virtual void Display() = 0;
	virtual void Search() = 0;
};