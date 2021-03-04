#ifndef CMANAGER_H

#define CMANAGER_H
#include "Global.h"
#include "Student.h"

class CManager {
private:
	static const char* mSubjectName[];
	CStudent* mStuLst[MAXSTUDENT];
	int mCount;
public:
	CManager();
	~CManager();

	void Input();
	void ShowData();
	void searchData();
	void ShowMenu();
	void Run();
};

#endif