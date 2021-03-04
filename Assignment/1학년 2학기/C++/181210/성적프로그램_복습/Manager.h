#ifndef CMANAGER_H
#define	CMANAGER_H
#include "Base.h"
#include "Student.h"

class cManager {
protected:
	cStudent*	mStudentList[MAXSTUDENT];
	int		mStuCount;
public:
	cManager();
	~cManager();
};
#endif
