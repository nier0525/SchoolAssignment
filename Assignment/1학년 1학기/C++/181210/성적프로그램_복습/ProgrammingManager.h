#ifndef CPROGRAMMING_MANAGER_H
#define CPROGRAMMING_MANAGER_H

#include "Student.h"
#include "Manager.h"

class cProgrammingManager : public cManager {
private:
	static const char*	mSubjectName[];
public:
	cProgrammingManager();
	~cProgrammingManager();

	void Input();
	void Display();
	void Search();
};

#endif