#ifndef CMANAGER_H
#define CMANAGER_H
#include "../Main/Global.h"
#include "../Student/Student.h"

class CManager
{
protected:
	CStudent*					mStudentList[MAXSTUDENT];
	int							mStuCount;
public:
	CManager();
	~CManager();
	
	virtual void Input() = 0;
	virtual void Display() = 0;
	virtual void Search() = 0;

};
#endif