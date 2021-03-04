#ifndef CDESIGN_MANAGER_H
#define CDESIGN_MANAGER_H
#include "../../Student/Student.h"
#include "../Manager.h"

class CDesignManager: public CManager
{
private:
	static const char*			mSubjectName[];	

public:
	CDesignManager();
	~CDesignManager();

	void Input();
	void Display();
	void Search();
};
#endif