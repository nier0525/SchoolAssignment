#ifndef CDESIGN_MANAGER_H
#define CDESIGN_MANAGER_H
#include "Student.h"
#include "Manager.h"

class cDesignManager : public cManager {
private:
	static const char*	mSubjectName[];
public:
	cDesignManager();
	~cDesignManager();

	void Input();
	void Display();
	void Search();
};
#endif