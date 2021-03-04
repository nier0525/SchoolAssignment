#ifndef CPROGRAMMING_MANAGER_H
#define CPROGRAMMING_MANAGER_H
#include "../../Student/Student.h"
#include "../Manager.h"

class CProgrammingManager :public CManager
{
private:
	static const char*			mSubjectName[];	

public:
	CProgrammingManager();
	~CProgrammingManager();

	void Input();
	void Display();
	void Search();
	
};
#endif